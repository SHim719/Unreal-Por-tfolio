#include "RPGNpcDatabase.h"
#include "RPGNpcData.h"

void URPGNpcDatabase::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	NpcTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Npc/DT_NpcTable"));
	
}

void URPGNpcDatabase::Deinitialize()
{
	Super::Deinitialize();
}

const FRPGNpcDataRow* URPGNpcDatabase::GetNpcDataRowByName(const FName& Name) const
{
	return NpcTable->FindRow<FRPGNpcDataRow>(Name, TEXT("Find_NpcDataRow"));
}

