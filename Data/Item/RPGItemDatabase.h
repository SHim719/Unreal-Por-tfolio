

#pragma once

#include "CoreMinimal.h"
#include "RPGItemDatabase.generated.h"

struct FRPGConsumeTooltipRow;
struct FRPGItemDefinition;
struct FRPGItemOptionRow;
class URPGItemData;

UCLASS()
class RPGPROJECT_API URPGItemDatabase : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	const FRPGItemDefinition* GetItemDataByName(const FName& Name) const;
private:
	UPROPERTY()
	TObjectPtr<URPGItemData> ItemData;
	
	UPROPERTY()
	TObjectPtr<UDataTable> ItemOptionTable;
};
