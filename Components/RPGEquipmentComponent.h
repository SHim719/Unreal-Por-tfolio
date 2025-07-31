

#pragma once

#include "CoreMinimal.h"

#include "RPGDefines.h"
#include "GameplayEffect.h"
#include "Components/ActorComponent.h"
#include "Data/ProcSystem/ProcSystemTypes.h"
#include "PlayerData/RPGPlayerDataManager.h"
#include "RPGEquipmentComponent.generated.h"


class URPGItemFragment_Equippable;
struct FRPGItemOptionInstance;
class URPGItemFragment_Attachable;
class UAbilitySystemComponent;
class UGameplayEffect;
struct FSlotInteractionMsg;
class ARPGEquipmentActor;
class ARPGPlayerCharacter;
struct FRPGItemDefinition;
class URPGItemInstance;

USTRUCT()
struct FRPGEquipmentEntry
{
	GENERATED_BODY()

public:
	URPGItemInstance* Equip(ARPGPlayerCharacter* PlayerCharacter, URPGItemInstance* EquipItem);
	void Unequip(UAbilitySystemComponent* ASC);

	URPGItemInstance* GetItemInstance() const { return ItemInstance; }
	
private:
	void Equip(ARPGPlayerCharacter* PlayerCharacter);
	
	void ApplyEquipStats(UAbilitySystemComponent* ASC, const TArray<FRPGItemOptionInstance>& ItemOptions);
	void EnrollProcEffects(UAbilitySystemComponent* ASC, const TArray<FRPGItemOptionInstance>& ItemOptions);
private:
	UPROPERTY()
	TObjectPtr<URPGItemInstance> ItemInstance;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> EquipEffectHandles;

	UPROPERTY()
	TArray<FRPGProcEffectInstance> ProcEffectInstances;
	
};

USTRUCT()
struct FRPGEquipmentList
{
	GENERATED_BODY()

public:
	URPGItemInstance* Equip(ARPGPlayerCharacter* PlayerCharacter, URPGItemInstance* EquipItem, const int32 SlotIndex);
	void Unequip(UAbilitySystemComponent* ASC, const int32 SlotIndex);
	
private:
	friend class URPGEquipmentComponent;
	
	UPROPERTY()
	TArray<FRPGEquipmentEntry> Entries;

};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API URPGEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void Initialize(const TArray<FRPGItemEntryData>& EquipmentDatas);
	URPGItemInstance* Equip(URPGItemInstance* EquipItem);
	void Unequip(const int32 SlotIndex);

	void GetEquipmentEntryDatas(TArray<FRPGItemEntryData>& EquipmentEntryDatas);

private:
	void BroadcastChangeEquipmentMsg(const URPGItemInstance* ItemInstance, bool bIsEquipped);
	int32 GetEquipSlotIndexFromItem(const URPGItemInstance* EquipItem) const;
	void  OnSlotInteraction(FGameplayTag Tag, const FSlotInteractionMsg& Msg);
	void  UpdateSlotWidget(UTexture2D* Thumbnail, const int32 SlotIndex, URPGItemInstance* ItemInstance) const;
	
private:
	UPROPERTY()
	FRPGEquipmentList EquipmentList;


private:
	virtual void BeginPlay() override;
};
