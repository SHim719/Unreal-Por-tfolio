


#include "RPGShopDatabase.h"

void URPGShopDatabase::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ShopTable = LoadObject<UDataTable>(this, TEXT("/Game/Data/Shop/DT_ShopTable"));
}

const FRPGShopDataRow* URPGShopDatabase::GetShopDataById(const FName& ShopId) const
{
	return ShopTable->FindRow<FRPGShopDataRow>(ShopId, TEXT("Find_ShopData"));
}
