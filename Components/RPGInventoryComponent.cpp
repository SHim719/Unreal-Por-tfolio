#include "Components/RPGInventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "RPGConsumeComponent.h"
#include "RPGEquipmentComponent.h"
#include "RPGHelperLibrary.h"
#include "RPGProject.h"
#include "Data/Item/RPGItemData.h"
#include "Message/RPGGameMessage.h"

int32 FRPGInventoryEntry::Init(URPGInventoryComponent* InventoryComponent, const FName& ItemId, const int32 ItemCount)
{
	URPGItemInstance* NewItemInstance = NewObject<URPGItemInstance>(InventoryComponent);
	NewItemInstance->Init(ItemId);
	
	return Init(NewItemInstance, ItemCount);
}

int32 FRPGInventoryEntry::Init(URPGItemInstance* InItemInstance, const int32 ItemCount)
{
	ItemInstance = InItemInstance;
	
	return AddStackCount(ItemCount);
}


int32 FRPGInventoryEntry::AddStackCount(const int32 AddCount)
{
	int32 RemainCount = 0;

	const int32 MaxStackCount = ItemInstance->GetMaxStackCount();
	StackCount += AddCount;
	if (StackCount > MaxStackCount)
	{
		RemainCount = StackCount - MaxStackCount;
		StackCount = MaxStackCount;
	}
	
	return RemainCount;
}

bool FRPGInventoryEntry::IsStackable() const
{
	return StackCount < ItemInstance->GetMaxStackCount();
}

void FRPGInventoryList::InsertItem(const FName& ItemId, const int32 ItemCount)
{
	// 먼저 인벤토리에 똑같은 아이템 있으면 개수 증가
	int32 RemainCount = StackSameItem(ItemId, ItemCount);
	
	if (RemainCount > 0)
	{
		RemainCount = InsertItemToEmptySlot(ItemId, RemainCount);
	}
}

void FRPGInventoryList::InsertItem(URPGItemInstance* ItemInstance, const int32 ItemCount)
{
	const int32 EmptySlotIndex = FindEmptySlot();
	if (EmptySlotIndex < Entries.Num())
	{
		Entries[EmptySlotIndex].Init(ItemInstance, ItemCount);

		UpdateSlotWidget(EmptySlotIndex);
	}
}

int32 FRPGInventoryList::CheckInsertItem(const FName& ItemId, const int32 ItemCount)
{
	int32 RemainCount = CheckStackItem(ItemId, ItemCount);
	
	if (RemainCount > 0)
	{
		RemainCount = CheckInsertItemToEmptySlot(ItemId, RemainCount);
	}

	return RemainCount;
	
}

int32 FRPGInventoryList::CheckStackItem(const FName& ItemId, const int32 ItemCount)
{
	const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(InventoryComponent, ItemId);
	if (ItemDef == nullptr)
		return ItemCount;
	
	int32 RemainCount = ItemCount;
	int32 SameSlotIndex = FindSameItemSlot(ItemId, 0);
	const int32 InventorySize = Entries.Num();

	while (RemainCount > 0 && SameSlotIndex < InventorySize)
	{
		const int32 CanStackCount = ItemDef->StackCount - Entries[SameSlotIndex].GetStackCount();
		RemainCount = FMath::Max(RemainCount - CanStackCount, 0);
		SameSlotIndex = FindSameItemSlot(ItemId, SameSlotIndex);
	}

	return RemainCount;
}

int32 FRPGInventoryList::CheckInsertItemToEmptySlot(const FName& ItemId, const int32 ItemCount)
{
	const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(InventoryComponent, ItemId);
	if (ItemDef == nullptr)
		return ItemCount;
	
	int32 RemainCount = ItemCount;
	int32 EmptySlotIndex = FindEmptySlot();
	const int32 InventorySize = Entries.Num();
	

	// 만약 최대로 쌓을 수 있는 개수가 99개인데 100개의 아이템이 들어왔을 경우를 가정, RemainCount가 0이될 때까지 반복
	while (RemainCount > 0 && EmptySlotIndex < InventorySize)
	{
		RemainCount = FMath::Max(RemainCount - ItemDef->StackCount, 0);
		EmptySlotIndex = FindEmptySlot();
	}
	
	return RemainCount;
}

void FRPGInventoryList::SwapItemSlot(const int32 SrcIndex, const int32 DstIndex)
{
	Swap(Entries[SrcIndex], Entries[DstIndex]);
	
	UpdateSlotWidget(SrcIndex);
	UpdateSlotWidget(DstIndex);
}

void FRPGInventoryList::InsertItemToSlot(URPGItemInstance* ItemInstance, const int32 SlotIndex,
                                         const int32 ItemCount)
{
	if (SlotIndex < Entries.Num())
	{
		if (ItemInstance)
		{
			Entries[SlotIndex].Init(ItemInstance, ItemCount);
		}
		else
		{
			Entries[SlotIndex].Clear();
		}
	}
	else
	{
		UE_LOG(LogRPGProject, Warning, TEXT("SlotIndex Overflow(FRPGInventoryList::InsertItemToSlot): %d"), SlotIndex);
	}

	UpdateSlotWidget(SlotIndex);
}

void FRPGInventoryList::AddStackCount(const int32 SlotIndex, const int32 Count)
{
	const int32 RemainCount = Entries[SlotIndex].AddStackCount(Count);

	const int32 CurStackCount = Entries[SlotIndex].GetStackCount();
	if (CurStackCount == 0)
	{
		Entries[SlotIndex].Clear();
	}
	
	// InventoryWidget에 슬롯 업데이트 메세지 전달.
	UpdateSlotWidget(SlotIndex);
	
}

void FRPGInventoryList::AddStackCount(const URPGItemInstance* ItemInstance, const int32 Count)
{
	for (int32 i = 0; i < Entries.Num(); ++i)
	{
		if (Entries[i].GetItemInstance() == ItemInstance)
		{
			Entries[i].AddStackCount(Count);
			UpdateSlotWidget(i);
			break;
		}
	}
}

bool FRPGInventoryList::HasItem(const FName& ItemId, const int32 Count) const
{
	int32 TargetCount = Count;
	for (const auto& Entry : Entries)
	{
		if (const URPGItemInstance* ItemInstance = Entry.GetItemInstance())
		{
			if (ItemInstance->IsSameItem(ItemId))
			{
				TargetCount -= Entry.GetStackCount();
			}

			if (TargetCount <= 0)
				return true;
		}
	}

	return false;
}

int32 FRPGInventoryList::GetItemCount(const FName& ItemId) const
{
	int32 Count = 0;
	for (const auto& Entry : Entries)
	{
		if (const URPGItemInstance* ItemInstance = Entry.GetItemInstance())
		{
			Count += Entry.GetStackCount();
		}
	}

	return Count;
}


int32 FRPGInventoryList::StackSameItem(const FName& ItemId, const int32 ItemCount)
{
	int32 RemainCount = ItemCount;
	int32 SameSlotIndex = FindSameItemSlot(ItemId, 0);
	const int32 InventorySize = Entries.Num();
	
	while (RemainCount > 0 && SameSlotIndex < InventorySize)
	{
		RemainCount = Entries[SameSlotIndex].AddStackCount(ItemCount);
		const int32 CurStackCount = Entries[SameSlotIndex].GetStackCount();
		
		// InventoryWidget에 슬롯 업데이트 메세지 전달.
		UpdateSlotWidget(SameSlotIndex);
		SameSlotIndex = FindSameItemSlot(ItemId, SameSlotIndex);
	}

	return RemainCount;
	
}

int32 FRPGInventoryList::FindSameItemSlot(const FName& ItemId, const int32 StartIndex)
{
	int32 Index = StartIndex;
	const int32 InventorySize = Entries.Num();
	for (; Index < InventorySize; ++Index)
	{
		const FRPGInventoryEntry& Entry = Entries[Index];
		const URPGItemInstance* ItemInstance = Entry.GetItemInstance();
		if (ItemInstance && ItemInstance->IsSameItem(ItemId) && Entry.IsStackable())
			break;
	}

	return Index;
}


int32 FRPGInventoryList::InsertItemToEmptySlot(const FName& ItemId, const int32 ItemCount)
{
	int32 RemainCount = ItemCount;
	int32 EmptySlotIndex = FindEmptySlot();
	const int32 InventorySize = Entries.Num();

	// 만약 최대로 쌓을 수 있는 개수가 99개인데 100개의 아이템이 들어왔을 경우를 가정, RemainCount가 0이될 때까지 반복
	while (RemainCount > 0 && EmptySlotIndex < InventorySize)
	{
		const int32 PrevRemainCount = RemainCount;
		
		RemainCount = Entries[EmptySlotIndex].Init(InventoryComponent, ItemId, RemainCount);

		UpdateSlotWidget(EmptySlotIndex);
		
		if (RemainCount > 0)
		{
			EmptySlotIndex = FindEmptySlot();
		}
	}
	
	return RemainCount;
}


int32 FRPGInventoryList::FindEmptySlot()
{
	const int32 InventorySize = Entries.Num();
	int32 Index = 0;
	for (; Index < InventorySize; ++Index)
	{
		if (Entries[Index].IsEmpty())
			return Index;
	}
	
	return Index;
}

void FRPGInventoryList::UpdateSlotWidget(const int32 SlotIndex)
{
	URPGItemInstance* ItemInstance = Entries[SlotIndex].GetItemInstance();
	
	FUpdateInventorySlotMsg UpdateSlotMsg(InventoryType, SlotIndex, nullptr, 0, nullptr);
	if (Entries[SlotIndex].IsEmpty() == false)
	{
		UpdateSlotMsg.Thumbnail = ItemInstance->GetItemDef()->IconTexture;
		UpdateSlotMsg.ItemCount = Entries[SlotIndex].GetStackCount();
		UpdateSlotMsg.TooltipContent = ItemInstance->GetTooltipContent();
	}
	
	RPGMessage::BroadcastMessage(InventoryComponent->GetWorld(), UpdateSlotMsg, FGameplayTag::RequestGameplayTag(FName("Widget.Inventory.UpdateSlot")));
	
	BroadcastEntryStateChanged(SlotIndex);
}

void FRPGInventoryList::BroadcastEntryStateChanged(const int32 SlotIndex)
{
	int32 ItemCount = 0;
	URPGItemInstance* ItemInstance = nullptr;
	if (Entries[SlotIndex].IsEmpty() == false)
	{
		ItemInstance = Entries[SlotIndex].GetItemInstance();
		ItemCount = Entries[SlotIndex].GetStackCount();
	}

	Entries[SlotIndex].OnEntryStateChanged.ExecuteIfBound(SlotIndex, ItemInstance, ItemCount);
	if (Entries[SlotIndex].IsEmpty())
	{
		Entries[SlotIndex].OnEntryStateChanged.Unbind();
	}
}

URPGInventoryComponent::URPGInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void URPGInventoryComponent::Initialize(const TArray<FRPGItemEntryData>& InventoryDatas, const int32 Gold)
{
	InitializeEntries();
	
	for (auto& ItemEntryData : InventoryDatas)
	{
		URPGItemInstance* ItemInstance = NewObject<URPGItemInstance>(this);
		ItemInstance->Init(ItemEntryData.ItemId, false);
		
		if (ItemEntryData.MainItemOptionInstances.Num() > 0)
		{
			ItemInstance->SetItemOptions(ItemEntryData.MainItemOptionInstances);
		}

		if (ItemEntryData.RandomItemOptionInstances.Num() > 0)
		{
			ItemInstance->SetRandomOptions(ItemEntryData.RandomItemOptionInstances);
		}

		if (ItemInstance->GetItemType() <= EItemType::Etc)
		{
			InventoryLists[static_cast<int32>(ItemInstance->GetItemType())].InsertItemToSlot(ItemInstance, ItemEntryData.SlotIndex, ItemEntryData.StackCount);
		}
	}

	AddGold(Gold);
}

void URPGInventoryComponent::InitializeEntries()
{
	InventoryLists.SetNum(static_cast<int32>(EItemType::End));
	for (int32 i = 0; i < static_cast<int32>(EItemType::End); ++i)
	{
		InventoryLists[i].Entries.SetNum(InventorySlotSize);
		InventoryLists[i].InventoryComponent = this;
		InventoryLists[i].InventoryType = static_cast<EItemType>(i);
	}

	FInitInventorySizeMsg InitSizeMsg(InventorySlotSize);
	RPGMessage::BroadcastMessage(GetWorld(), InitSizeMsg, FGameplayTag::RequestGameplayTag(TEXT("Widget.Inventory.UpdateSlotSize")));
}

void URPGInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	RPGMessage::RegisterListener(GetWorld(), FGameplayTag::RequestGameplayTag(TEXT("Widget.Slot.Interaction")), this, &ThisClass::HandleSlotInteractionMsg);
	RPGMessage::RegisterListener(GetWorld(), FGameplayTag::RequestGameplayTag(TEXT("Widget.Slot.DragDrop")), this, &ThisClass::HandleDragDropMsg);
}




void URPGInventoryComponent::InsertItem(const FName ItemId, const int32 ItemCount)
{
	const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(this, ItemId);
	if (ItemDef == nullptr)
		return;

	if (ItemDef->ItemType == EItemType::Gold)
	{
		AddGold(ItemCount);
	}
	else if (ItemDef->ItemType == EItemType::Exp)
	{
		if (ARPGPlayerState* PS = RPGHelper::GetPlayerState_Safe(this))
		{
			PS->GiveExp(ItemCount);
		}
	}
	else
	{
		InventoryLists[static_cast<int32>(ItemDef->ItemType)].InsertItem(ItemId, ItemCount);
	}

	BroadcastObtainItem(ItemId, ItemCount);
}

void URPGInventoryComponent::InsertItem(URPGItemInstance* ItemInstance, const int32 ItemCount)
{
	return InventoryLists[static_cast<int32>(ItemInstance->GetItemType())].InsertItem(ItemInstance, ItemCount);
}

bool URPGInventoryComponent::CanInsertItem(const FName& ItemId, const int32 ItemCount)
{
	const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(this, ItemId);
	if (ItemDef == nullptr)
		return false;

	if (ItemDef->ItemType == EItemType::Gold || ItemDef->ItemType == EItemType::Exp)
		return true;
	
	return InventoryLists[static_cast<int32>(ItemDef->ItemType)].CheckInsertItem(ItemId, ItemCount) == 0;
}

int32 URPGInventoryComponent::GetItemCount(const FName& ItemId) const
{
	const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(this, ItemId);
	if (ItemDef == nullptr)
		return false;

	return InventoryLists[static_cast<int32>(ItemDef->ItemType)].GetItemCount(ItemId);
}

void URPGInventoryComponent::AddGold(const int32 Gold)
{
	CurrentGold = FMath::Clamp(CurrentGold + Gold, 0, INT32_MAX);
	BroadcastGoldChanged(CurrentGold);
}

bool URPGInventoryComponent::HasItem(const FName& ItemId, const int32 Count) const
{
	const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(this, ItemId);
	if (ItemDef == nullptr)
		return false;

	return InventoryLists[static_cast<int32>(ItemDef->ItemType)].HasItem(ItemId, Count);
}


void URPGInventoryComponent::AddStackCount(const URPGItemInstance* ItemInstance, const int32 Count)
{
	if (ItemInstance == nullptr || Count == 0)
		return ;

	const EItemType ItemType = ItemInstance->GetItemType();
	if (ItemType <= EItemType::Etc)
		InventoryLists[static_cast<int32>(ItemType)].AddStackCount(ItemInstance, Count);
}

void URPGInventoryComponent::GetItemEntryDatasForSave(TArray<FRPGItemEntryData>& InventoryEntryDatas)
{
	for (int32 i = 0; i < InventoryLists.Num(); ++i)
	{
		int32 Index = 0;
		for (auto& Entry : InventoryLists[i].Entries)
		{
			if (Entry.GetItemInstance())
			{
				FRPGItemEntryData ItemEntryData;
				ItemEntryData.ItemId = Entry.GetItemInstance()->GetItemId();
				ItemEntryData.SlotIndex = Index;
				ItemEntryData.MainItemOptionInstances = Entry.GetItemInstance()->GetItemOptions();
				ItemEntryData.RandomItemOptionInstances = Entry.GetItemInstance()->GetRandomOptions();
				ItemEntryData.StackCount = Entry.GetStackCount();
				InventoryEntryDatas.Add(ItemEntryData);
			}
			Index++;
		}
	}
}


bool URPGInventoryComponent::GetInventoryEntry(const EItemType ItemType, const int32 SlotIndex,
                                               FRPGInventoryEntry& InventoryEntry)
{
	if (SlotIndex >= InventorySlotSize)
		return false;

	InventoryEntry = InventoryLists[static_cast<int32>(ItemType)].Entries[SlotIndex];
	return true;
}

void URPGInventoryComponent::HandleDragDropMsg(FGameplayTag Tag, const FDragDropMsg& Msg)
{
	// 인벤토리 <-> 인벤토리, 인벤토리 <-> 장비끼리만 Swap 허용
	if (Msg.FromSlotType > ESlotType::Equip || Msg.ToSlotType >= ESlotType::Equip)
		return;
	
	if (Msg.FromSlotType <= ESlotType::Inventory_Etc)
	{
		// 인벤토리 <-> 인벤토리는 같은 카테고리의 슬롯끼리 이동 가능.
		if (Msg.FromSlotType == Msg.ToSlotType && Msg.FromSlotIndex != Msg.ToSlotIndex)
		{
			InventoryLists[static_cast<int32>(Msg.FromSlotType)].SwapItemSlot(Msg.FromSlotIndex, Msg.ToSlotIndex);;
		}
	}
	else if (Msg.FromSlotType == ESlotType::Equip)
	{
		
	}
	
}

void URPGInventoryComponent::HandleSlotInteractionMsg(FGameplayTag Tag, const FSlotInteractionMsg& Msg)
{
	if (Msg.SlotType == ESlotType::Inventory_Equip)
	{
		EquipItem(Msg.SlotIndex);
	}
	else if (Msg.SlotType == ESlotType::Inventory_Consume)
	{
		ConsumeItem(Msg.SlotIndex);
	}
}

void URPGInventoryComponent::BroadcastGoldChanged(const int32 Gold)
{
	FGoldChangedMsg Msg;
	Msg.Gold = Gold;

	RPGMessage::BroadcastMessage(this, Msg, FGameplayTag::RequestGameplayTag(FName("Inventory.UpdateGold")));
}

void URPGInventoryComponent::BroadcastObtainItem(const FName& ItemId, const int32 Count)
{
	FQuestTaskSignalMsg QuestSignalMsg;
	QuestSignalMsg.TaskActionType = ERPGQuestTaskActionType::ObtainItem;
	QuestSignalMsg.TargetId = ItemId;
	QuestSignalMsg.Count = Count;
	RPGMessage::BroadcastMessage(this, QuestSignalMsg, FGameplayTag::RequestGameplayTag(FName("Quest.Notify.TaskSignal")));
	
}

void URPGInventoryComponent::EquipItem(const int32 InventorySlotIndex)
{
	URPGItemInstance* ItemInstance = InventoryLists[static_cast<int32>(EItemType::Equipment)].Entries[InventorySlotIndex].GetItemInstance();
	if (ItemInstance == nullptr || ItemInstance->GetItemType() != EItemType::Equipment)
		return;

	if (URPGEquipmentComponent* EquipmentComponent = GetOwner()->GetComponentByClass<URPGEquipmentComponent>())
	{
		URPGItemInstance* SwappedItemInstance = EquipmentComponent->Equip(ItemInstance);
		InventoryLists[static_cast<int32>(EItemType::Equipment)].InsertItemToSlot(SwappedItemInstance, InventorySlotIndex);
	}
}

void URPGInventoryComponent::ConsumeItem(const int32 InventorySlotIndex)
{
	URPGItemInstance* ItemInstance = InventoryLists[static_cast<int32>(EItemType::Consume)].Entries[InventorySlotIndex].GetItemInstance();
	if (ItemInstance == nullptr || ItemInstance->GetItemType() != EItemType::Consume)
		return;

	if (const URPGConsumeComponent* ConsumeComponent = GetOwner()->GetComponentByClass<URPGConsumeComponent>())
	{
		ConsumeComponent->Consume(ItemInstance, InventorySlotIndex);
		
		InventoryLists[static_cast<int32>(EItemType::Consume)].AddStackCount(InventorySlotIndex, -1);
		BroadcastObtainItem(ItemInstance->GetItemId(), -1);
	}
}




