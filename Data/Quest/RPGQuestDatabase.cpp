// 


#include "RPGQuestDatabase.h"
#include "RPGQuestData.h"

void URPGQuestDatabase::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	QuestData = LoadObject<URPGQuestData>(nullptr, TEXT("/Game/Data/Quest/DA_QuestData"));
}

const FRPGQuestDefinition* URPGQuestDatabase::GetQuestDefinitionByName(const FName& QuestId) const
{
	return QuestData->FindQuestDefinitionByName(QuestId);
	
}


