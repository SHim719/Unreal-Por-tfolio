#include "RPGQuestTrackerListView.h"

#include "RPGHelperLibrary.h"
#include "RPGQuestTrackerView.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Message/RPGGameMessage.h"
#include "Quest/RPGQuestManager.h"

void URPGQuestTrackerListView::NativeOnInitialized()
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
			CreateQuestTrackerView(Pair.Key);
		}
	}
}

void URPGQuestTrackerListView::HandleOnAcceptQuest(FGameplayTag Tag, const FOnAcceptQuestMsg& Msg)
{
	if (QuestTrackerViewClass == nullptr || Grid == nullptr)
		return;

	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this, Msg]()
	{
		if (URPGQuestTrackerView* QuestTrackerView = CreateWidget<URPGQuestTrackerView>(this, QuestTrackerViewClass))
		{
			QuestTrackerView->Init(Msg.QuestId);
			QuestTrackerViews.Add(QuestTrackerView);
			QuestTrackerViewSlots.Add(Grid->AddChildToUniformGrid(QuestTrackerView, QuestTrackerViews.Num() - 1, 0));
		}
	}));
}

void URPGQuestTrackerListView::HandleOnDropQuest(FGameplayTag Tag, const FOnDropQuestMsg& Msg)
{
	RemoveQuestTrackerView(Msg.QuestId);
	RearrangeQuestTrackerViews();
}

void URPGQuestTrackerListView::HandleOnCompleteQuest(FGameplayTag Tag, const FOnCompleteQuestMsg& Msg)
{
	RemoveQuestTrackerView(Msg.QuestId);
	RearrangeQuestTrackerViews();
}

void URPGQuestTrackerListView::CreateQuestTrackerView(const FName& QuestId)
{
	if (URPGQuestTrackerView* QuestTrackerView = CreateWidget<URPGQuestTrackerView>(this, QuestTrackerViewClass))
	{
		QuestTrackerView->Init(QuestId);
		QuestTrackerViews.Add(QuestTrackerView);
		QuestTrackerViewSlots.Add(Grid->AddChildToUniformGrid(QuestTrackerView, QuestTrackerViews.Num() - 1, 0));
	}
}

void URPGQuestTrackerListView::RemoveQuestTrackerView(const FName& QuestId)
{
	for (int32 i = 0; i < QuestTrackerViews.Num(); ++i)
	{
		if (QuestTrackerViews[i] && QuestTrackerViews[i]->GetRefQuestId() == QuestId)
		{
			Grid->RemoveChild(QuestTrackerViews[i]);
			QuestTrackerViews.RemoveAt(i);
			QuestTrackerViewSlots.RemoveAt(i);
			break;
		}
	}
}

void URPGQuestTrackerListView::RearrangeQuestTrackerViews()
{
	for (int32 i = 0; i < QuestTrackerViewSlots.Num(); ++i)
	{
		if (QuestTrackerViewSlots[i])
		{
			QuestTrackerViewSlots[i]->SetRow(i);
		}
	}
}
