// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/RPGQuickBarComponent.h"
#include "Data/Item/RPGItemOption.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGPlayerDataManager.generated.h"

USTRUCT()
struct FRPGItemEntryData
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 SlotIndex = 0;
	
	UPROPERTY()
	FName ItemId;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY()
	TArray<FRPGItemOptionInstance> MainItemOptionInstances;

	UPROPERTY()
	TArray<FRPGItemOptionInstance> RandomItemOptionInstances;
	
};


UCLASS()
class RPGPROJECT_API URPGPlayerDataManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void SaveCurrentPlayerData();
	void LoadCurrentPlayerData();

	const TArray<FRPGItemEntryData>& GetInventoryDatas() { return InventoryDatas;}
	const TArray<FRPGItemEntryData>& GetEquipmentDatas() { return EquipmentDatas;}
	
private:
	TMap<FGameplayTag, int32> AbilityPoints;
	TMap<FGameplayTag, float> CooldownInfos;
	
	TArray<FRPGItemEntryData> InventoryDatas;
	TArray<FRPGItemEntryData> EquipmentDatas;

	int32 CurrentGold = 0;

	TArray<FRefSlotInfo> QuickBarRefSlotInfoDatas;

	int32 Level = 1;
	int32 SkillPoint = 1;
	
	float Hp = -1;
	float Exp = -1;

	
};
