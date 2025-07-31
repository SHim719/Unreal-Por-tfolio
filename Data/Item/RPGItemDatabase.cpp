
#include "RPGItemDatabase.h"
#include "RPGItemData.h"
#include "RPGItemOption.h"


void URPGItemDatabase::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ItemData = LoadObject<URPGItemData>(nullptr, TEXT("/Game/Data/Item/DA_ItemData"));
	check(ItemData);
}

void URPGItemDatabase::Deinitialize()
{
	Super::Deinitialize();
}

const FRPGItemDefinition* URPGItemDatabase::GetItemDataByName(const FName& Name) const
{
	return ItemData->FindItemDefinitionByName(Name);
}

