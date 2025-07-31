

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGShopDatabase.generated.h"



USTRUCT()
struct FRPGShopDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> SellItemIds;
	
};


UCLASS()
class RPGPROJECT_API URPGShopDatabase : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const FRPGShopDataRow* GetShopDataById(const FName& ShopId) const;
	
	
private:	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> ShopTable;
};


