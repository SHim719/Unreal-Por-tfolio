

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RPGDefines.h"
#include "Components/ActorComponent.h"
#include "Item/RPGItemInstance.h"
#include "PlayerData/RPGPlayerDataManager.h"
#include "RPGInventoryComponent.generated.h"

DECLARE_DELEGATE_ThreeParams(FOnEntryStateChanged, const int32 SlotIndex, URPGItemInstance* ItemInstance, const int32 Count)

struct FDragDropMsg;
struct FSlotInteractionMsg;

USTRUCT()
struct FRPGInventoryEntry
{
	GENERATED_BODY()

public:
	int32 Init(URPGInventoryComponent* InventoryComponent, const FName& ItemName, const int32 ItemCount);
	int32 Init(URPGItemInstance* InItemInstance, const int32 ItemCount);
	
	URPGItemInstance* GetItemInstance() const { return ItemInstance; }
	int32 GetStackCount() const { return StackCount; }
	int32 AddStackCount(const int32 AddCount);
	bool IsStackable() const;
	bool IsEmpty() const { return ItemInstance == nullptr;}
	void Clear() { ItemInstance = nullptr; StackCount = 0;}

	FOnEntryStateChanged OnEntryStateChanged;
	
private:
	friend class URPGQuickBarComponent;
	
	UPROPERTY()
	TObjectPtr<URPGItemInstance> ItemInstance;
	
	int32 StackCount = 0;
};

USTRUCT()
struct FRPGInventoryList
{
	GENERATED_BODY()

public:
	void InsertItem(const FName& ItemName, const int32 ItemCount);
	void InsertItem(URPGItemInstance* ItemInstance, const int32 ItemCount);
	void SwapItemSlot(const int32 SrcIndex, const int32 DstIndex);
	void InsertItemToSlot(URPGItemInstance* ItemInstance, const int32 SlotIndex, const int32 ItemCount = 1);
	void AddStackCount(const int32 SlotIndex, const int32 Count);
	void AddStackCount(const URPGItemInstance* ItemInstance, const int32 Count);
	
	int32 CheckInsertItem(const FName& ItemId, const int32 ItemCount);
	
	bool HasItem(const FName& ItemId, const int32 Count) const;
	int32 GetItemCount(const FName& ItemId) const;
	
	void BroadcastEntryStateChanged(const int32 SlotIndex);
	
private:
	int32 StackSameItem(const FName& ItemId, const int32 ItemCount);
	int32 CheckStackItem(const FName& ItemId, const int32 ItemCount);
	int32 FindSameItemSlot(const FName& ItemId, const int32 StartIndex);
	
	int32 InsertItemToEmptySlot(const FName& ItemId, const int32 ItemCount);
	int32 CheckInsertItemToEmptySlot(const FName& ItemId, const int32 ItemCount);
	int32 FindEmptySlot();

	void UpdateSlotWidget(const int32 SlotIndex);

private:
	friend class URPGInventoryComponent;
	friend class URPGQuickBarComponent;
	
	UPROPERTY()
	TObjectPtr<URPGInventoryComponent> InventoryComponent;
	
	UPROPERTY()
	TArray<FRPGInventoryEntry> Entries;

	EItemType InventoryType = EItemType::End;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API URPGInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URPGInventoryComponent();
	void Initialize(const TArray<FRPGItemEntryData>& InventoryDatas, const int32 Gold);
	
private:
	virtual void BeginPlay() override;
	void InitializeEntries();
	

public:
	UFUNCTION(BlueprintCallable)
	void InsertItem(const FName ItemId, const int32 ItemCount);
	void InsertItem(URPGItemInstance* ItemInstance, const int32 ItemCount = 1);
	bool CanInsertItem(const FName& ItemId, const int32 ItemCount);
	bool HasItem(const FName& ItemId, const int32 Count) const;
	int32 GetItemCount(const FName& ItemId) const;
	bool HasGold(const int32 Gold) const { return CurrentGold >= Gold;}
	void AddGold(const int32 Gold); 
	int32 GetCurrentGold() const { return CurrentGold;}
	void AddStackCount(const URPGItemInstance* ItemInstance, const int32 Count);
	void GetItemEntryDatasForSave(TArray<FRPGItemEntryData>& InventoryEntryDatas);

	bool GetInventoryEntry(const EItemType ItemType, const int32 SlotIndex, FRPGInventoryEntry& InventoryEntry);
	FOnEntryStateChanged& GetEntryStateChangedDelegate(const EItemType ItemType, const int32 SlotIndex) { return InventoryLists[static_cast<int32>(ItemType)].Entries[SlotIndex].OnEntryStateChanged; }
private:
	void HandleDragDropMsg(FGameplayTag Tag, const FDragDropMsg& Msg);
	void HandleSlotInteractionMsg(FGameplayTag Tag, const FSlotInteractionMsg& Msg);

	void BroadcastGoldChanged(const int32 Gold);
	void BroadcastObtainItem(const FName& ItemId, const int32 Count);
	
	void EquipItem(const int32 InventorySlotIndex);
	void ConsumeItem(const int32 InventorySlotIndex);

private:
	friend class URPGQuickBarComponent;
	
	UPROPERTY()
	TArray<FRPGInventoryList> InventoryLists;
	
	UPROPERTY(EditDefaultsOnly)
	int32 InventorySlotSize = 0;

	int32 CurrentGold = 0;

	
};
