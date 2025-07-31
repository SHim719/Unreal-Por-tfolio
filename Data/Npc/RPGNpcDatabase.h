// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPGNpcDatabase.generated.h"

struct FRPGNpcDataRow;

UCLASS()
class RPGPROJECT_API URPGNpcDatabase : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	const FRPGNpcDataRow* GetNpcDataRowByName(const FName& Name) const;

private:
	UPROPERTY()
	TObjectPtr<UDataTable> NpcTable;
};
