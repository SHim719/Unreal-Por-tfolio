#include "RPGPlayerQuestListWidget.h"

#include "RPGHelperLibrary.h"
#include "RPGQuestInfoWidget.h"
#include "RPGQuestRowWidget.h"
#include "Components/VerticalBox.h"
#include "Message/RPGGameMessage.h"
#include "Quest/RPGQuestManager.h"

void URPGPlayerQuestListWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Quest.Action.Accept")), this, &ThisClass::HandleOnAcceptQuest);
	RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Quest.Action.Drop")), this, &ThisClass::HandleOnDropQuest);
	RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Quest.Action.Complete")), this, &ThisClass::HandleOnCompleteQuest);


	if (URPGQuestManager* QuestManager = RPGHelper::GetQuestManager(this))
	{
		const TMap<FName, TObjectPtr<URPGQuestInstance>>& OnGoingQuests = QuestManager->GetOngoingQuests();
		for (auto& Pair : OnGoingQuests)
		{
			AddQuestRow(Pair.Key);
		}
	}
}

void URPGPlayerQuestListWidget::HandleOnAcceptQuest(FGameplayTag Tag, const FOnAcceptQuestMsg& Msg)
{
	AddQuestRow(Msg.QuestId);
}

void URPGPlayerQuestListWidget::HandleOnDropQuest(FGameplayTag Tag, const FOnDropQuestMsg& Msg)
{
	RemoveQuestRow(Msg.QuestId);
}

void URPGPlayerQuestListWidget::HandleOnCompleteQuest(FGameplayTag Tag, const FOnCompleteQuestMsg& Msg)
{
	RemoveQuestRow(Msg.QuestId);
}

void URPGPlayerQuestListWidget::AddQuestRow(const FName& QuestId)
{
	if (URPGQuestRowWidget* QuestRowWidget = CreateWidget<URPGQuestRowWidget>(this, QuestRowWidgetClass))
	{
		if (VerticalBox_Quests)
		{
			QuestRowWidget->SetRefQuestId(QuestId);
			QuestRowWidget->OnClickedQuestRowButton.BindUObject(this, &URPGPlayerQuestListWidget::ShowQuestInfo);
			VerticalBox_Quests->AddChildToVerticalBox(QuestRowWidget);
			QuestRowWidgets.Add(QuestRowWidget);
		}
	}
}

void URPGPlayerQuestListWidget::ShowQuestInfo(const FName& QuestId)
{
	if (QuestInfoWidget)
	{
		QuestInfoWidget->UpdateQuestInfo(QuestId, ERPGQuestState::Ongoing);
		QuestInfoWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void URPGPlayerQuestListWidget::RemoveQuestRow(const FName& QuestId)
{
	for (int32 i = 0; i < QuestRowWidgets.Num(); ++i)
	{
		if (QuestRowWidgets[i] && QuestRowWidgets[i]->GetRefQuestId() == QuestId)
		{
			if (VerticalBox_Quests)
			{
				VerticalBox_Quests->RemoveChildAt(i);
				QuestRowWidgets.RemoveAt(i);
				return;
			}
		}
	}
}
