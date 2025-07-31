#include "RPGNpcQuestListWidget.h"

#include "RPGHelperLibrary.h"
#include "RPGQuestInfoWidget.h"
#include "RPGQuestRowWidget.h"
#include "Components/VerticalBox.h"
#include "Data/Npc/RPGNpcData.h"
#include "Dialogue/RPGDialogueManager.h"
#include "Message/RPGGameMessage.h"
#include "Quest/RPGQuestManager.h"


class URPGQuestManager;

void URPGNpcQuestListWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Quest.Action.Accept")),
	   this, &ThisClass::HandleOnAcceptQuest);

	RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Quest.Action.Complete")),
	   this, &ThisClass::HandleOnCompleteQuest);

	RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Widget.Quest.ShowNpcQuestInfo")),
	   this, &ThisClass::HandleShowQuestInfoMsg);
	
}

void URPGNpcQuestListWidget::InitializeQuestList(const FName& NpcId, const TArray<FName>& GivableQuestIds, const TArray<FName>& WaitCompleteQuestIds)
{
	if (QuestRowWidgetClass == nullptr)
		return;

	CurrentNpcId = NpcId;
	
	if (VerticalBox_CanAccept)
	{
		VerticalBox_CanAccept->ClearChildren();

		for (const FName& GivableQuestId : GivableQuestIds)
		{
			if (URPGQuestRowWidget* QuestRow = CreateWidget<URPGQuestRowWidget>(this, QuestRowWidgetClass))
			{
				QuestRow->SetRefQuestId(GivableQuestId);
				QuestRow->SetVisibility(ESlateVisibility::Visible);
				QuestRow->OnClickedQuestRowButton.BindUObject(this, &ThisClass::StartQuestDialogue);
				VerticalBox_CanAccept->AddChildToVerticalBox(QuestRow);
				QuestRowWidgets_CanAccept.Add(QuestRow);
			}
		}
	}

	if (VerticalBox_CanComplete)
	{
		VerticalBox_CanComplete->ClearChildren();

		for (const FName& WaitCompleteQuestId : WaitCompleteQuestIds)
		{
			if (URPGQuestRowWidget* QuestRow = CreateWidget<URPGQuestRowWidget>(this, QuestRowWidgetClass))
			{
				QuestRow->SetRefQuestId(WaitCompleteQuestId);
				QuestRow->OnClickedQuestRowButton.BindUObject(this, &ThisClass::StartQuestDialogue);
				VerticalBox_CanComplete->AddChildToVerticalBox(QuestRow);
				QuestRowWidgets_CanComplete.Add(QuestRow);
			}
		}
	}
}


void URPGNpcQuestListWidget::HandleOnAcceptQuest(FGameplayTag Tag, const FOnAcceptQuestMsg& Msg)
{
	if (VerticalBox_CanAccept == nullptr)
		return;
	
	for (int32 i = 0; i < QuestRowWidgets_CanAccept.Num(); ++i)
	{
		if (QuestRowWidgets_CanAccept[i] && Msg.QuestId == QuestRowWidgets_CanAccept[i]->GetRefQuestId())
		{
			QuestRowWidgets_CanAccept.RemoveAt(i);
			VerticalBox_CanAccept->RemoveChildAt(i);
		}
	}
	
}

void URPGNpcQuestListWidget::HandleOnCompleteQuest(FGameplayTag Tag, const FOnCompleteQuestMsg& Msg)
{
	if (VerticalBox_CanComplete == nullptr)
		return;
	
	for (int32 i = 0; i < QuestRowWidgets_CanComplete.Num(); ++i)
	{
		if (QuestRowWidgets_CanComplete[i] && Msg.QuestId == QuestRowWidgets_CanComplete[i]->GetRefQuestId())
		{
			QuestRowWidgets_CanComplete.RemoveAt(i);
			VerticalBox_CanComplete->RemoveChildAt(i);
		}
	}
}

void URPGNpcQuestListWidget::HandleShowQuestInfoMsg(FGameplayTag, const FShowNpcQuestInfoMsg& Msg)
{
	ShowQuestInfoWidget(Msg.QuestId);
}


void URPGNpcQuestListWidget::StartQuestDialogue(const FName& QuestId)
{
	URPGQuestManager* QuestManager = RPGHelper::GetQuestManager(this);
	URPGDialogueManager* DialogueManager = RPGHelper::GetDialogueManager(this);
	if (QuestManager == nullptr || DialogueManager == nullptr)
		return;

	const FRPGQuestDefinition* QuestDef = RPGHelper::GetQuestDefinition_Safe(this, QuestId);
	const FRPGNpcDataRow* NpcData = RPGHelper::GetNpcData_Safe(this, CurrentNpcId);
	if (QuestDef == nullptr)
		return;

	URPGQuestInstance* QuestInstance = QuestManager->GetQuestInstance(QuestId);
	if (QuestInstance && QuestInstance->GetQuestState() == ERPGQuestState::WaitCompletion)
	{
		DialogueManager->StartDialogue(NpcData->DisplayName, QuestDef->CompletionDialogue);
	}
	else
	{
		DialogueManager->StartDialogue(NpcData->DisplayName, QuestDef->StartDialogue);	
	}
}

void URPGNpcQuestListWidget::ShowQuestInfoWidget(const FName& QuestId)
{
	QuestInfoWidget->SetVisibility(ESlateVisibility::Visible);
	
	URPGQuestManager* QuestMgr = RPGHelper::GetQuestManager(this);
	if (QuestMgr == nullptr)
		return;

	ERPGQuestState QuestState = ERPGQuestState::Inactive;
	auto* QuestInstance = QuestMgr->GetQuestInstance(QuestId);
	if (QuestInstance && QuestInstance->GetQuestState() == ERPGQuestState::WaitCompletion)
	{
		QuestState = ERPGQuestState::WaitCompletion; 
	}

	QuestInfoWidget->UpdateQuestInfo(QuestId, QuestState);
}


