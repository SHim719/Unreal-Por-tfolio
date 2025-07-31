// 


#include "RPGNpcInteractionManager.h"

#include "RPGHelperLibrary.h"
#include "Components/Button.h"
#include "Data/Npc/RPGNpcData.h"
#include "UI/RPGMainWidget.h"
#include "UI/DialogueBox/RPGDialogueBox.h"
#include "UI/Quest/RPGNpcQuestListWidget.h"
#include "UI/Shop/RPGShopWidget.h"

void URPGNpcInteractionManager::Initialize(FSubsystemCollectionBase& Collection)
{
	RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("NPCInteraction.StartInteraction")),
	this, &ThisClass::HandleStartInteraction);
	RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Widget.Quest.UpdateNpcQuestList")),
		this, &ThisClass::HandleUpdateQuestListMsg);
	
	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &ThisClass::Init);
	
}

void URPGNpcInteractionManager::Init(UWorld* World, const UWorld::InitializationValues)
{
	World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
	{
		if (ARPGHUD* HUD = RPGHelper::GetHUD_Safe(this))
		{
			if (auto* MainWidget = HUD->GetMainWidget())
			{
				DialogueBox = Cast<URPGDialogueBox>(MainWidget->GetWidgetFromName(FName(TEXT("DialogueBox"))));
				if (DialogueBox && DialogueBox->Button_Exit)
				{
					DialogueBox->Button_Exit->OnClicked.AddDynamic(this, &ThisClass::OnClicked_ExitButton);
				}

				NpcQuestList = Cast<URPGNpcQuestListWidget>(MainWidget->GetWidgetFromName(FName(TEXT("NpcQuestList"))));
				NpcShopWidget = Cast<URPGShopWidget>(MainWidget->GetWidgetFromName(FName(TEXT("NpcShopWidget"))));
			}
		}
	}));
}

void URPGNpcInteractionManager::OnClicked_ExitButton()
{
	DialogueBox->SetVisibility(ESlateVisibility::Collapsed);
	NpcQuestList->SetVisibility(ESlateVisibility::Collapsed);
	NpcShopWidget->SetVisibility(ESlateVisibility::Collapsed);

	CurInteractionNpcId = NAME_None;
}

void URPGNpcInteractionManager::HandleStartInteraction(FGameplayTag Tag, const FStartNpcInteractionMsg& Msg)
{
	if (const FRPGNpcDataRow* NpcData = RPGHelper::GetNpcData_Safe(this, Msg.NpcId))
	{
		CurInteractionNpcId = Msg.NpcId;
		
		if (DialogueBox)
		{
			DialogueBox->SetVisibility(ESlateVisibility::Visible);
			DialogueBox->Init(NpcData->DisplayName, NpcData->InitialDialogue);
		}

		if (NpcQuestList && (Msg.GivableQuestIds.IsEmpty() == false || Msg.WaitCompleteQuestIds.IsEmpty() == false))
		{
			NpcQuestList->SetVisibility(ESlateVisibility::Visible);
			NpcQuestList->InitializeQuestList(Msg.NpcId, Msg.GivableQuestIds, Msg.WaitCompleteQuestIds);
		}
	
		if (NpcData->ShopId != NAME_None)
		{
			NpcShopWidget->SetVisibility(ESlateVisibility::Visible);
			NpcShopWidget->Init(NpcData->ShopId);
		}
	}
}

void URPGNpcInteractionManager::HandleUpdateQuestListMsg(FGameplayTag, const FUpdateNpcQuestListMsg& Msg)
{
	if (Msg.NpcId != CurInteractionNpcId)
		return;

	if (NpcQuestList && NpcQuestList->GetVisibility() != ESlateVisibility::Collapsed)
	{
		NpcQuestList->InitializeQuestList(CurInteractionNpcId, Msg.GivableQuestIds, Msg.WaitCompleteQuestIds);
	}
	
}
