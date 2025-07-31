#include "RPGQuestInfoWidget.h"
#include "RPGHelperLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Data/Item/RPGItemData.h"
#include "Message/RPGGameMessage.h"
#include "Quest/RPGQuestManager.h" 
#include "Quest/QuestTask/RPGQuestTask.h"
#include "UI/Slot/RPGGridSlots.h"


void URPGQuestInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Exit)
	{
		Button_Exit->OnClicked.AddDynamic(this, &ThisClass::OnClicked_Exit);
	}
}

void URPGQuestInfoWidget::UpdateQuestInfo(const FName& QuestId, const ERPGQuestState QuestState)
{
	const FRPGQuestDefinition* QuestDef = RPGHelper::GetQuestDefinition_Safe(this, QuestId);
	if (QuestDef == nullptr)
		return;

	if (Text_QuestTitle)
	{
		Text_QuestTitle->SetText(QuestDef->QuestTitle);
	}

	if (Text_Description)
	{
		Text_Description->SetText(QuestDef->QuestDescription);
	}

	if (Button_AcceptOrDropOrComplete && Text_AcceptOrDropOrComplete)
	{
		Text_AcceptOrDropOrComplete->SetText(QuestButtonStateInfos[QuestState]);
		Button_AcceptOrDropOrComplete->OnClicked.Clear();
		switch (QuestState)
		{
		case ERPGQuestState::Inactive:
			Button_AcceptOrDropOrComplete->OnClicked.AddDynamic(this, &ThisClass::OnClicked_Accept);
			break;
		case ERPGQuestState::Ongoing:
			Button_AcceptOrDropOrComplete->OnClicked.AddDynamic(this, &ThisClass::OnClicked_Drop);
			break;
		case ERPGQuestState::WaitCompletion:
			Button_AcceptOrDropOrComplete->OnClicked.AddDynamic(this, &ThisClass::OnClicked_Complete);
			break;
		default:
			return;
		}
	}

	if (QuestDef->OverrideTaskDesc.IsEmpty())
	{
		UpdateQuestTaskTexts(QuestId, QuestState);
	}
	else
	{
		Text_Tasks->SetText(QuestDef->OverrideTaskDesc);
	}
		
	UpdateRewardInfo(QuestDef->QuestRewards);

	RefQuestId = QuestId;
}

void URPGQuestInfoWidget::UpdateQuestTaskTexts(const FName& QuestId, const ERPGQuestState QuestState)
{
	if (Text_Tasks == nullptr)
		return;
	
	if (QuestState == ERPGQuestState::Inactive)
	{
		if (const auto* QuestDef = RPGHelper::GetQuestDefinition_Safe(this, QuestId))
		{
			UpdateQuestTaskTexts(QuestDef->QuestTasks);
		}
	}
	else
	{
		if (URPGQuestManager* QuestManager = RPGHelper::GetQuestManager(this))
		{
			if (URPGQuestInstance* QuestInstance = QuestManager->GetQuestInstance(QuestId))
			{
				UpdateQuestTaskTexts(QuestInstance->GetCurrentTasks());
			}
		}
	}
	
}

void URPGQuestInfoWidget::UpdateQuestTaskTexts(const TArray<URPGQuestTask*>& QuestTasks)
{
	TArray<FText> TaskTexts;
	TaskTexts.Reserve(QuestTasks.Num());
	
	for (URPGQuestTask* Task : QuestTasks)
	{
		FText QuestTaskText = Task->GetCurrentQuestTaskText();
		TaskTexts.Add(QuestTaskText);
	}
	Text_Tasks->SetText(FText::Join(FText::FromString(TEXT("\n")), TaskTexts));
}

void URPGQuestInfoWidget::UpdateRewardInfo(const TArray<FRPGQuestReward>& QuestRewards)
{
	if (Grid_Rewards == nullptr)
		return;

	Grid_Rewards->HideAllSlots();

	for (int32 i = 0; i < QuestRewards.Num(); ++i)
	{
		auto* SlotWidget = Grid_Rewards->GetSlotWidget(i);
		const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(this, QuestRewards[i].RewardItemId);
		if (SlotWidget && ItemDef)
		{
			SlotWidget->SetVisibility(ESlateVisibility::Visible);
			SlotWidget->UpdateSlot(ItemDef->IconTexture, QuestRewards[i].RewardCount);
			SlotWidget->UpdateTooltipContent(RPGHelper::GetItemTooltipContent(this, QuestRewards[i].RewardItemId));
		}
	}
}


void URPGQuestInfoWidget::OnClicked_Accept()
{
	if (URPGQuestManager* QuestManager = RPGHelper::GetQuestManager(this))
	{
		QuestManager->AcceptQuest(RefQuestId);
	}
	
	SetVisibility(ESlateVisibility::Collapsed);
}

void URPGQuestInfoWidget::OnClicked_Drop()
{
	if (URPGQuestManager* QuestManager = RPGHelper::GetQuestManager(this))
	{
		QuestManager->DropQuest(RefQuestId);
	}
	
	SetVisibility(ESlateVisibility::Collapsed);
}

void URPGQuestInfoWidget::OnClicked_Complete()
{
	auto* QuestMgr = RPGHelper::GetQuestManager(this);
	if (QuestMgr == nullptr)
		return;

	if (QuestMgr->CheckCanReceiveReward(RefQuestId))
	{
		QuestMgr->CompleteQuest(RefQuestId);
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		// TODO: 퀘스트 완료 실패
	}
}

void URPGQuestInfoWidget::OnClicked_Exit()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
