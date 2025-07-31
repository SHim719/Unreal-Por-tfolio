#include "Data/Enemy/RPGEnemyDatabase.h"

void URPGEnemyDatabase::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	EnemyTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Enemy/DT_EnemyTable"));
	check(EnemyTable);
}

void URPGEnemyDatabase::Deinitialize()
{
	Super::Deinitialize();
}

const FRPGEnemyDataRow* URPGEnemyDatabase::GetEnemyData(const FName& EnemyId) const
{
	return EnemyTable->FindRow<FRPGEnemyDataRow>(EnemyId, TEXT("Find_EnemyDataRow"));
}
