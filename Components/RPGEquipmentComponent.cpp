#include "Components/RPGEquipmentComponent.h"

#include "RPGHelperLibrary.h"
#include "RPGInventoryComponent.h"
#include "RPGProcComponent.h"
#include "RPGProject.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Actor/RPGEquipmentActor.h"
#include "Character/RPGPlayerCharacter.h"
#include "Data/Item/RPGItemData.h"
#include "Item/RPGItemInstance.h"
#include "Item/Fragment/RPGItemFragment_Armor.h"
#include "Item/Fragment/RPGItemFragment_Equippable.h"
#include "Item/Fragment/RPGItemFragment_Weapon.h"
#include "Message/RPGGameMessage.h"


URPGItemInstance* FRPGEquipmentEntry::Equip(ARPGPlayerCharacter* PlayerCharacter, URPGItemInstance* EquipItem)
{
	URPGItemInstance* SwappedItem = ItemInstance;
	
	if (SwappedItem)
	{
		Unequip(RPGHelper::GetPlayerAbilitySystemComponent_Safe(PlayerCharacter));
	}

	ItemInstance = EquipItem;
	Equip(PlayerCharacter);
	

	return SwappedItem;
}

void FRPGEquipmentEntry::Equip(ARPGPlayerCharacter* PlayerCharacter)
{
	const URPGItemFragment_Equippable* Fragment_Equippable = ItemInstance->FindFragmentByClass<URPGItemFragment_Equippable>();
	if (Fragment_Equippable == nullptr)
		return;

	if (URPGAbilitySystemComponent* ASC = RPGHelper::GetPlayerAbilitySystemComponent_Safe(PlayerCharacter))
	{
		ApplyEquipStats(ASC, ItemInstance->GetItemOptions());
		ApplyEquipStats(ASC, ItemInstance->GetRandomOptions());
		EnrollProcEffects(ASC, ItemInstance->GetItemOptions());
		EnrollProcEffects(ASC, ItemInstance->GetRandomOptions());
	}
	
}



void FRPGEquipmentEntry::ApplyEquipStats(UAbilitySystemComponent* ASC, const TArray<FRPGItemOptionInstance>& ItemOptions)
{
	if (ASC == nullptr)
		return;

	for (const FRPGItemOptionInstance& ItemOptionInstance : ItemOptions)
	{
		const FRPGItemOptionRow* ItemOptionRow = ItemOptionInstance.OptionDataRowHandle.GetRow<FRPGItemOptionRow>(TEXT("Query ItemOption"));
		if (ItemOptionRow == nullptr)
			continue;

		if (ItemOptionRow->OptionType == ERPGItemOptionType::PassiveStatModifer && ItemOptionRow->ToApplyStatModifierEffect)
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ItemOptionRow->ToApplyStatModifierEffect, 0.f, EffectContextHandle);
			FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
			if (Spec)
			{
				for (const TPair<FGameplayTag, int32>& OptionValue : ItemOptionInstance.OptionValues)
				{
					const float Value = Spec->GetSetByCallerMagnitude(OptionValue.Key);
					Spec->SetSetByCallerMagnitude(OptionValue.Key, Value + OptionValue.Value);
				}
				EquipEffectHandles.Add(ASC->ApplyGameplayEffectSpecToSelf(*Spec));
			}
		}
	}
}

void FRPGEquipmentEntry::EnrollProcEffects(UAbilitySystemComponent* ASC, const TArray<FRPGItemOptionInstance>& ItemOptions) 
{
	for (const FRPGItemOptionInstance& ItemOptionInstance : ItemOptions)
	{
		const FRPGItemOptionRow* ItemOptionRow = ItemOptionInstance.OptionDataRowHandle.GetRow<FRPGItemOptionRow>(TEXT("Query ItemOption"));
		if (ItemOptionRow == nullptr)
			continue;
		
		if (ItemOptionRow->OptionType == ERPGItemOptionType::ProcEffect && ItemOptionRow->ProcEffectDef.EffectToApply != nullptr)
		{
			FRPGProcEffectInstance ProcEffectInstance;
			ProcEffectInstance.TriggerTag = ItemOptionRow->ProcEffectDef.TriggerTag;
			ProcEffectInstance.Target = ItemOptionRow->ProcEffectDef.Target;

			FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ItemOptionRow->ProcEffectDef.EffectToApply, 1.f, ContextHandle);	

			if (FGameplayEffectSpec* Spec = SpecHandle.Data.Get())
			{
				for (const TPair<FGameplayTag, int32>& OptionValue : ItemOptionInstance.OptionValues)
				{
					if (OptionValue.Key == FGameplayTag::RequestGameplayTag(FName("Effect.Percentage")))
					{
						ProcEffectInstance.ChancePercentage = OptionValue.Value;
					}
					else
					{
						Spec->SetSetByCallerMagnitude(OptionValue.Key, OptionValue.Value);
					}
				}
			}
			ProcEffectInstance.ApplyEffectSpecHandle = SpecHandle;
			ProcEffectInstances.Add(ProcEffectInstance);
			
			if (URPGProcComponent* ProcComponent = ASC->GetOwner()->FindComponentByClass<URPGProcComponent>())
			{
				ProcComponent->AddProcEffect(ProcEffectInstance);
			}
		}
	}
	
}

void FRPGEquipmentEntry::Unequip(UAbilitySystemComponent* ASC)
{
	if (ItemInstance == nullptr || ASC == nullptr)
		return;

	for (auto& EquipEffectHandle : EquipEffectHandles)
	{
		ASC->RemoveActiveGameplayEffect(EquipEffectHandle);
	}

	if (URPGProcComponent* ProcComponent = ASC->GetOwner()->FindComponentByClass<URPGProcComponent>())
	{
		for (auto& ProcEffectInstance : ProcEffectInstances)
		{
			ProcComponent->RemoveProcEffect(ProcEffectInstance);
		}
	}

	EquipEffectHandles.Empty();
	ProcEffectInstances.Empty();
	ItemInstance = nullptr;
}

URPGItemInstance* FRPGEquipmentList::Equip(ARPGPlayerCharacter* PlayerCharacter, URPGItemInstance* EquipItem, const int32 SlotIndex)
{
	if (PlayerCharacter)
	{
		if (SlotIndex < Entries.Num())
		{
			return Entries[SlotIndex].Equip(PlayerCharacter, EquipItem);
		}
		else
		{
			UE_LOG(LogRPGProject, Warning, TEXT("SlotIndex Range Overflow! : %d"), SlotIndex);
		}
	}
	else
	{
		UE_LOG(LogRPGProject, Warning, TEXT("PlayerCharacter is nullptr"));
	}
	
	
	return nullptr;
}

void FRPGEquipmentList::Unequip(UAbilitySystemComponent* ASC, const int32 SlotIndex)
{
	if (SlotIndex < Entries.Num())
	{
		Entries[SlotIndex].Unequip(ASC);
	}
	else
	{
		UE_LOG(LogRPGProject, Warning, TEXT("SlotIndex Range Overflow! : %d"), SlotIndex);
	}
}

void URPGEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	RPGMessage::RegisterListener(GetWorld(), FGameplayTag::RequestGameplayTag(TEXT("Widget.Slot.Interaction")), this, &ThisClass::OnSlotInteraction);
}

void URPGEquipmentComponent::Initialize(const TArray<FRPGItemEntryData>& EquipmentDatas)
{
	EquipmentList.Entries.SetNum(static_cast<int32>(EEquipSlotType::End));

	for (auto& EquipmentData : EquipmentDatas)
	{
		URPGItemInstance* ItemInstance = NewObject<URPGItemInstance>(this);
		ItemInstance->Init(EquipmentData.ItemId, false);

		ItemInstance->SetItemOptions(EquipmentData.MainItemOptionInstances);
		ItemInstance->SetRandomOptions(EquipmentData.RandomItemOptionInstances);

		ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(RPGHelper::GetPlayerCharacter_Safe(this));
		Equip(ItemInstance);
	}
}

URPGItemInstance* URPGEquipmentComponent::Equip(URPGItemInstance* EquipItem)
{
	const int32 SlotIndex = GetEquipSlotIndexFromItem(EquipItem);
	ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(RPGHelper::GetPlayerCharacter_Safe(this));

	UpdateSlotWidget(EquipItem->GetItemDef()->IconTexture, SlotIndex, EquipItem);
	BroadcastChangeEquipmentMsg(EquipItem, true);
	
	return EquipmentList.Equip(PlayerCharacter, EquipItem, SlotIndex);
}

void URPGEquipmentComponent::Unequip(const int32 SlotIndex)
{	
	ARPGPlayerState* PlayerState = RPGHelper::GetPlayerState_Safe(this);
	if (PlayerState == nullptr)
		return;

	URPGInventoryComponent* InventoryComponent = PlayerState->FindComponentByClass<URPGInventoryComponent>();
	if (InventoryComponent == nullptr)
		return;
	
	URPGItemInstance* UnequippedItemInstance = EquipmentList.Entries[SlotIndex].GetItemInstance();
	if (InventoryComponent && UnequippedItemInstance)
	{
		if (InventoryComponent->CanInsertItem(UnequippedItemInstance->GetItemId(), 1) == true)
		{
			InventoryComponent->InsertItem(UnequippedItemInstance, 1);
			EquipmentList.Unequip(PlayerState->GetAbilitySystemComponent(), SlotIndex);
			UpdateSlotWidget(nullptr, SlotIndex, nullptr);
			BroadcastChangeEquipmentMsg(UnequippedItemInstance, false);
		}
	}
}

void URPGEquipmentComponent::GetEquipmentEntryDatas(TArray<FRPGItemEntryData>& EquipmentEntryDatas)
{
	for (int32 i = 0; i < EquipmentList.Entries.Num(); ++i)
	{
		if (auto* ItemInstance = EquipmentList.Entries[i].GetItemInstance())
		{
			FRPGItemEntryData ItemEntryData;
			ItemEntryData.ItemId = ItemInstance->GetItemId();
			ItemEntryData.SlotIndex = i;
			ItemEntryData.MainItemOptionInstances = ItemInstance->GetItemOptions();
			ItemEntryData.RandomItemOptionInstances = ItemInstance->GetRandomOptions();
			ItemEntryData.StackCount = 1;
			EquipmentEntryDatas.Add(ItemEntryData);
		}
	}
}


int32 URPGEquipmentComponent::GetEquipSlotIndexFromItem(const URPGItemInstance* EquipItem) const 
{
	const URPGItemFragment_Equippable* Fragment_Equippable = EquipItem->FindFragmentByClass<URPGItemFragment_Equippable>();
	if (Fragment_Equippable == nullptr)
		return -1;

	switch (const EEquipmentType EquipType = Fragment_Equippable->EquipmentType)
	{
	case EEquipmentType::Armor:
	{
		const URPGItemFragment_Armor* Fragment_Armor = Cast<URPGItemFragment_Armor>(Fragment_Equippable);
		return static_cast<int32>(Fragment_Armor->ArmorType);
	}
	case EEquipmentType::Weapon:
	{
		return static_cast<int32>(EEquipSlotType::Weapon);
	}
	}
	
	return -1;
}

void URPGEquipmentComponent::OnSlotInteraction(FGameplayTag Tag, const FSlotInteractionMsg& Msg)
{
	if (Msg.SlotType == ESlotType::Equip)
		Unequip(Msg.SlotIndex);
}

void URPGEquipmentComponent::UpdateSlotWidget(UTexture2D* Thumbnail, const int32 SlotIndex, URPGItemInstance* ItemInstance) const
{
	RPGMessage::BroadcastMessage(GetWorld(), FUpdateEquipmentSlotMsg(SlotIndex, Thumbnail, ItemInstance == nullptr ? nullptr : ItemInstance->GetTooltipContent())
		, FGameplayTag::RequestGameplayTag("Widget.Equipment.UpdateSlot"));
}

void URPGEquipmentComponent::BroadcastChangeEquipmentMsg(const URPGItemInstance* ItemInstance, bool bIsEquipped)
{
	FOnPlayerChangeEquipmentMsg ChangeEquipmentMsg;
	ChangeEquipmentMsg.EquipItemInstance = ItemInstance;
	ChangeEquipmentMsg.bIsEquipped = bIsEquipped;
	RPGMessage::BroadcastMessage(this, ChangeEquipmentMsg, FGameplayTag::RequestGameplayTag(FName(TEXT("Player.ChangeEquipment"))));
}
