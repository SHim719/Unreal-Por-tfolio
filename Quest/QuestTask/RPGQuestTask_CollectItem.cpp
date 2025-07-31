#include "RPGQuestTask_CollectItem.h"

#include "RPGHelperLibrary.h"
#include "Character/RPGPlayerCharacter.h"
#include "Components/RPGInventoryComponent.h"
#include "Message/RPGGameMessage.h"


void URPGQuestTask_CollectItem::StartTask()
{
	if (auto* PlayerState = RPGHelper::GetPlayerState_Safe(this))
	{
		if (URPGInventoryComponent* Inventory = PlayerState->FindComponentByClass<URPGInventoryComponent>())
		{
			for (const FName& ItemId : TargetItemIds)
			{
				CurCollectCount += Inventory->GetItemCount(ItemId);
			}
		}
	}

}

bool URPGQuestTask_CollectItem::ReceiveNotifyTask(const FQuestTaskSignalMsg& Msg)
{
	if (TaskActionType != Msg.TaskActionType)
		return false;

	for (const FName& ItemId : TargetItemIds)
	{
		if (Msg.TargetId == ItemId)
		{
			CurCollectCount += Msg.Count;
			return true;;
		}
	}

	return false;
}

URPGQuestTask* URPGQuestTask_CollectItem::Clone(UObject* WorldContextObject)
{
	URPGQuestTask_CollectItem* QuestTask = DuplicateObject<URPGQuestTask_CollectItem>(this, WorldContextObject);
	if (QuestTask == nullptr)
		return nullptr;

	return QuestTask;
}

FText URPGQuestTask_CollectItem::GetCurrentQuestTaskText()
{
	FFormatNamedArguments Arguments;
	Arguments.Add("CurCollectCount", CurCollectCount);
	Arguments.Add("TargetCollectCount", TargetCollectCount);

	TArray<FText> TaskTexts;
	TaskTexts.Add(TaskDescription); 
	TaskTexts.Add(FText::FromString(TEXT("({CurCollectCount}/{TargetCollectCount})")));
	
	return FText::Format(FText::Join(FText::GetEmpty(), TaskTexts), Arguments);
}
