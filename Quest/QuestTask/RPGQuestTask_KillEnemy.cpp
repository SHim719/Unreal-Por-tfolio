#include "RPGQuestTask_KillEnemy.h"

#include "Message/RPGGameMessage.h"

bool URPGQuestTask_KillEnemy::ReceiveNotifyTask(const FQuestTaskSignalMsg& Msg)
{
	if (TaskActionType != Msg.TaskActionType)
		return false;

	for (const FName& EnemyId : TargetEnemyIds)
	{
		if (Msg.TargetId == EnemyId)
		{
			CurHuntCount = FMath::Min(CurHuntCount + Msg.Count, HuntCount);
			return true;
		}
	}

	return false;
}

URPGQuestTask* URPGQuestTask_KillEnemy::Clone(UObject* WorldContextObject)
{
	URPGQuestTask_KillEnemy* QuestTask = DuplicateObject<URPGQuestTask_KillEnemy>(this, WorldContextObject);
	if (QuestTask == nullptr)
		return nullptr;

	return QuestTask;
}

FText URPGQuestTask_KillEnemy::GetCurrentQuestTaskText()
{
	FFormatNamedArguments Arguments;
	Arguments.Add("CurHuntCount", CurHuntCount);
	Arguments.Add("HuntCount", HuntCount);

	TArray<FText> TaskTexts;
	TaskTexts.Add(TaskDescription); 
	TaskTexts.Add(FText::FromString(TEXT("({CurHuntCount}/{HuntCount})")));
	
	return FText::Format(FText::Join(FText::GetEmpty(), TaskTexts), Arguments);
	
}