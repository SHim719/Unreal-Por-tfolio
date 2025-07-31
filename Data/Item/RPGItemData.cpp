
#include "RPGItemData.h"
#include "UObject/ObjectSaveContext.h"



URPGItemData::URPGItemData()
{
	
}

const FRPGItemDefinition* URPGItemData::FindItemDefinitionByName(const FName& ItemName) const
{
	return ItemDefinitions.Find(ItemName);
}
