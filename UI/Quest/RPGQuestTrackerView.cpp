#include "RPGQuestTrackerView.h"
#include "RPGHelperLibrary.h"
#include "Components/TextBlock.h"
#include "Data/Quest/RPGQuestData.h"
#include "Quest/RPGQuestInstance.h"
#include "Quest/RPGQuestManager.h"
#include "Quest/QuestTask/RPGQuestTask.h"


void URPGQuestTrackerView::Init(const FName& QuestId)
{
	const FRPGQuestDefinition* QuestDef = RPGHelper::GetQuestDefinition_Safe(this, QuestId);
	if (QuestDef == nullptr)
		return;

	if (Text_QuestTitle)
	{
		Text_QuestTitle->SetText(QuestDef->QuestTitle);
	}
	
	if (const auto* QuestMgr = RPGHelper::GetQuestManager(this))
	{
		RefQuestId = QuestId;
		RefQuestInstance = QuestMgr->GetQuestInstance(QuestId);
		if (RefQuestInstance)
		{
			RefQuestInstance->OnQuestTaskStateChanged.BindUObject(this, &ThisClass::UpdateTaskText);
		}
	}

	if (QuestDef->QuestTasks.IsEmpty() == false)
	{
		TaskTexts.Reserve(QuestDef->QuestTasks.Num());
		for (auto& QuestTask : RefQuestInstance->GetCurrentTasks())
		{
			if (QuestTask)
			{
				TaskTexts.Add(QuestTask->GetCurrentQuestTaskText());
			}
		}
		
		if (Text_Tasks)
		{
			Text_Tasks->SetText(FText::Join(FText::FromString(TEXT("\n")), TaskTexts));
		}
		
	}
	else
	{
		if (Text_Tasks)
		{
			Text_Tasks->SetText(QuestDef->OverrideTaskDesc);
		}
	}
	
}

void URPGQuestTrackerView::UpdateTaskText(const int32 TaskIndex)
{
	if (RefQuestInstance && TaskIndex < TaskTexts.Num())
	{
		const TArray<TObjectPtr<URPGQuestTask>>& Tasks = RefQuestInstance->GetCurrentTasks();
		TaskTexts[TaskIndex] = Tasks[TaskIndex]->GetCurrentQuestTaskText();
	}

	if (Text_Tasks)
	{
		Text_Tasks->SetText(FText::Join(FText::FromString(TEXT("\n")), TaskTexts));
	}
}
