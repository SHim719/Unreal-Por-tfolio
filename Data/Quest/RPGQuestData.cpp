// 


#include "RPGQuestData.h"

URPGQuestData::URPGQuestData()
{
}

const FRPGQuestDefinition* URPGQuestData::FindQuestDefinitionByName(const FName& QuestName) const
{
	return NameToQuest.Find(QuestName);
}
