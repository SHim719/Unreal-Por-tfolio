#include "Quest/RPGQuestInstance.h"

#include "RPGGameplayTags.h"
#include "RPGHelperLibrary.h"
#include "Data/Quest/RPGQuestData.h"
#include "Message/RPGGameMessage.h"
#include "QuestTask/RPGQuestTask.h"

void URPGQuestInstance::Init(const FName& QuestId)
{
	const FRPGQuestDefinition* QuestDef = RPGHelper::GetQuestDefinition_Safe(this, QuestId);
	if (QuestDef == nullptr)
		return;

	RefQuestId = QuestId;
	
	for (URPGQuestTask* QuestTask : QuestDef->QuestTasks)
	{
		if (URPGQuestTask* ClonedTask = QuestTask->Clone(this))
		{
			Tasks.Add(ClonedTask);
			ClonedTask->StartTask();
			if (ClonedTask->IsComplete()) 
			{
				CompletedTaskCount++;
			}
		}
	}
	ConfirmQuestState();

	QuestNotifyHandler = RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Quest.Notify.TaskSignal")),
		this, &ThisClass::HandleTaskSignalMsg);
}

void URPGQuestInstance::BeginDestroy()
{
	Super::BeginDestroy();

	QuestNotifyHandler.Unregister();
}

void URPGQuestInstance::ConfirmQuestState()
{
	const ERPGQuestState PrevQuestState = QuestState;
	
	if (CompletedTaskCount == Tasks.Num())
	{
		QuestState = ERPGQuestState::WaitCompletion;
	}
	else
	{
		QuestState = ERPGQuestState::Ongoing;
	}

	if (PrevQuestState != QuestState)
	{
		OnQuestStateChanged.ExecuteIfBound(RefQuestId, QuestState);
	}
		
}

void URPGQuestInstance::HandleTaskSignalMsg(FGameplayTag Tag, const FQuestTaskSignalMsg& Msg)
{
	CompletedTaskCount = 0;
	for (int32 i = 0; i < Tasks.Num(); ++i)
	{
		if (Tasks[i])
		{
			if (Tasks[i]->ReceiveNotifyTask(Msg) == true)
			{
				OnQuestTaskStateChanged.ExecuteIfBound(i);
			}
			
			if (Tasks[i]->IsComplete())
				CompletedTaskCount++;
		}
	}
	ConfirmQuestState();
}

