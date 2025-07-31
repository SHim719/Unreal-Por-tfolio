#include "Components/RPGQuickBarComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "RPGInventoryComponent.h"
#include "RPGProject.h"
#include "RPGSkillTreeComponent.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Message/RPGGameMessage.h"
#include "Skill/RPGSkillInstance.h"


URPGQuickBarComponent::URPGQuickBarComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URPGQuickBarComponent::Initialize(const TArray<FRefSlotInfo>& InRefSlotInfos)
{
	OwnerASC = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()));
	if (OwnerASC == nullptr)
	{
		UE_LOG(LogRPGProject, Warning, TEXT("Can't Find ASC(URPGQuickBarComponent)"));
	}

	InventoryComponent = Cast<URPGInventoryComponent>(GetOwner()->FindComponentByClass<URPGInventoryComponent>());
	if (InventoryComponent == nullptr)
	{
		UE_LOG(LogRPGProject, Warning, TEXT("Can't Find InventoryComponent(URPGQuickBarComponent)"));
	}

	SkillTreeComponent = Cast<URPGSkillTreeComponent>(GetOwner()->FindComponentByClass<URPGSkillTreeComponent>());
	if (SkillTreeComponent == nullptr)
	{
		UE_LOG(LogRPGProject, Warning, TEXT("Can't Find SkillTreeComponent(URPGQuickBarComponent)"));
	}

	if (InRefSlotInfos.Num() > 0)
	{
		RefSlotInfos = InRefSlotInfos;

		for (int32 i = 0; i < RefSlotInfos.Num(); ++i)
		{
			if (RefSlotInfos[i].RefSlotType == ESlotType::Inventory_Consume)
			{
				BindConsumeEntryChanged(i, RefSlotInfos[i].RefSlotIndex);
			}
			else if (RefSlotInfos[i].RefSlotType == ESlotType::Skill)
			{
				BindSkill(i);
			}
		}
	}
	else
	{
		RefSlotInfos.SetNum(SlotCount);
	}

	RPGMessage::RegisterListener(GetWorld(), FGameplayTag::RequestGameplayTag(TEXT("Widget.Slot.DragDrop")), this, &ThisClass::HandleDragDropMsg);
	
}


void URPGQuickBarComponent::KeyInputStarted(const int32 SlotIndex)
{
	if (SlotIndex < SlotCount)
	{
		if (RefSlotInfos[SlotIndex].RefSlotType == ESlotType::Inventory_Consume)
		{
			InventoryComponent->ConsumeItem(RefSlotInfos[SlotIndex].RefSlotIndex);
		}
		else if (RefSlotInfos[SlotIndex].RefSlotType == ESlotType::Skill)
		{
			if (URPGSkillInstance* SkillInstance = SkillTreeComponent->GetSkillInstance(RefSlotInfos[SlotIndex].RefSlotIndex))
			{
				SkillInstance->OnInputPressed();
			}
		}
	}
}

void URPGQuickBarComponent::KeyInputReleased(const int32 SlotIndex)
{
	if (SlotIndex < SlotCount)
	{
		if (RefSlotInfos[SlotIndex].RefSlotType == ESlotType::Inventory_Consume)
		{
			
		}
		else if (RefSlotInfos[SlotIndex].RefSlotType == ESlotType::Skill)
		{
			if (URPGSkillInstance* SkillInstance = SkillTreeComponent->GetSkillInstance(RefSlotInfos[SlotIndex].RefSlotIndex))
			{
				SkillInstance->OnInputReleased();
			}
		}
	}
}

void URPGQuickBarComponent::HandleDragDropMsg(FGameplayTag Tag, const FDragDropMsg& Msg)
{
	if ((Msg.FromSlotType != ESlotType::Inventory_Consume && Msg.FromSlotType != ESlotType::Skill && Msg.FromSlotType != ESlotType::Quick)
		|| Msg.ToSlotType != ESlotType::Quick)
		return;

	if (Msg.FromSlotType == ESlotType::Inventory_Consume)
	{
		HandleBindConsumeItem(Msg);
	}
	else if (Msg.FromSlotType == ESlotType::Skill)
	{
		HandleBindSkill(Msg);
	}
	else
	{
		HandleSwapSlot(Msg);
	}
}

void URPGQuickBarComponent::HandleBindConsumeItem(const FDragDropMsg& Msg)
{
	const int32 QuickSlotIndex = Msg.ToSlotIndex;
	RefSlotInfos[QuickSlotIndex].RefSlotType = ESlotType::Inventory_Consume;
	RefSlotInfos[QuickSlotIndex].RefSlotIndex = Msg.FromSlotIndex;

	BindConsumeEntryChanged(QuickSlotIndex, Msg.FromSlotIndex);
}

void URPGQuickBarComponent::HandleBindSkill(const FDragDropMsg& Msg)
{
	RefSlotInfos[Msg.ToSlotIndex].RefSlotType = ESlotType::Skill;
	RefSlotInfos[Msg.ToSlotIndex].RefSlotIndex = Msg.FromSlotIndex;

	BindSkill(Msg.ToSlotIndex);
}

void URPGQuickBarComponent::HandleSwapSlot(const FDragDropMsg& Msg)
{
	Swap(RefSlotInfos[Msg.FromSlotIndex], RefSlotInfos[Msg.ToSlotIndex]);
	UpdateSlot(Msg.FromSlotIndex);
	UpdateSlot(Msg.ToSlotIndex);
}

void URPGQuickBarComponent::BindConsumeEntryChanged(const int32 QuickSlotIndex, const int32 RefSlotIndex)
{
	// Entry 변화에 따른 퀵 슬롯 동기화를 위한 Delegate 바인딩
	InventoryComponent->GetEntryStateChangedDelegate(EItemType::Consume, RefSlotIndex).BindLambda(
		[this, QuickSlotIndex](const int32 SlotIndex, URPGItemInstance* ItemInstance, const int32 Count)
		{
			UTexture2D* IconTexture = nullptr;
			const URPGTooltipContent* TooltipContent = nullptr;
			
			if (ItemInstance)
			{
				IconTexture = ItemInstance->GetItemDef()->IconTexture;
				TooltipContent = ItemInstance->GetTooltipContent();
				RefSlotInfos[QuickSlotIndex].RefSlotIndex = SlotIndex;
			}
			else
			{
				RefSlotInfos[QuickSlotIndex].Clear();
			}
			
			UpdateSlotWidget(QuickSlotIndex, IconTexture, Count, TooltipContent);
		});

	// 초기화.
	InventoryComponent->InventoryLists[static_cast<int32>(EItemType::Consume)].BroadcastEntryStateChanged(RefSlotIndex);
}

void URPGQuickBarComponent::BindSkill(const int32 QuickSlotIndex)
{
	if (auto* SkillInstance = SkillTreeComponent->GetSkillInstance(RefSlotInfos[QuickSlotIndex].RefSlotIndex))
	{
		SkillInstance->EnrolledQuickSlotIndex = QuickSlotIndex;
		SkillInstance->OnCooldownChanged.AddUObject(this, &ThisClass::HandleCooldownChange);
	}
	
	UpdateSlot(QuickSlotIndex);
}

void URPGQuickBarComponent::HandleCooldownChange(float RemainingTime, float CooldownTime, const int32 QuickSlotIndex)
{
	const FUpdateQuickBarCooldownMsg Msg(QuickSlotIndex, RemainingTime, CooldownTime);
	RPGMessage::BroadcastMessage(this, Msg, FGameplayTag::RequestGameplayTag("Widget.QuickBar.UpdateCooldown"));
}

void URPGQuickBarComponent::UpdateSlot(const int32 QuickSlotIndex)
{
	if (RefSlotInfos[QuickSlotIndex].RefSlotType == ESlotType::Inventory_Consume)
	{
		BindConsumeEntryChanged(QuickSlotIndex, RefSlotInfos[QuickSlotIndex].RefSlotIndex);
	}
	else if (RefSlotInfos[QuickSlotIndex].RefSlotType == ESlotType::Skill)
	{
		const FGameplayTag& SkillTag = SkillTreeComponent->GetSkillTagByIndex(RefSlotInfos[QuickSlotIndex].RefSlotIndex);
		URPGSkillInstance* SkillInstance = SkillTreeComponent->GetSkillInstance(RefSlotInfos[QuickSlotIndex].RefSlotIndex);
		if (SkillTag.IsValid())
		{
			if (const FRPGSkillDataRow* SkillData = GetWorld()->GetGameInstance()->GetSubsystem<URPGSkillDatabase>()->GetSkillDataByTag(SkillTag))
			{
				UpdateSlotWidget(QuickSlotIndex, SkillData->MainSkillIcon, 1, nullptr);
			}
		}
	}
	else if (RefSlotInfos[QuickSlotIndex].RefSlotType == ESlotType::End)
	{
		RefSlotInfos[QuickSlotIndex].Clear();
		UpdateSlotWidget(QuickSlotIndex, nullptr, 0, nullptr);
	}
}

void URPGQuickBarComponent::UpdateSlotWidget(const int32 QuickSlotIndex, UTexture2D* Thumbnail, const int32 Count, const URPGTooltipContent* TooltipContent)
{
	const FUpdateQuickBarSlotMsg Msg(QuickSlotIndex, Thumbnail, Count, TooltipContent);
	RPGMessage::BroadcastMessage(this, Msg, FGameplayTag::RequestGameplayTag("Widget.QuickBar.UpdateSlot"));
}







