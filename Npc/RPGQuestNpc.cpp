#include "RPGQuestNpc.h"

#include "RPGHelperLibrary.h"
#include "Components/WidgetComponent.h"
#include "Data/Npc/RPGNpcData.h"
#include "Data/Quest/RPGQuestData.h"
#include "Message/RPGGameMessage.h"
#include "Quest/RPGQuestManager.h"
#include "UI/Npc/RPGNpcDescTextWidget.h"


ARPGQuestNpc::ARPGQuestNpc()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARPGQuestNpc::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateGivableQuests();
	UpdateWaitCompletionQuests();
	
	QuestMessageHandlers.Add(FGameplayTag::RequestGameplayTag(FName("Quest.Action.Drop")), RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Quest.Action.Drop")),
	this, &ThisClass::HandleOnDropQuest));

	QuestMessageHandlers.Add(FGameplayTag::RequestGameplayTag(FName("Quest.Action.Complete")), RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Quest.Action.Complete")),
	this, &ThisClass::HandleOnCompleteQuest));
	
	QuestMessageHandlers.Add(FGameplayTag::RequestGameplayTag(FName("Quest.ChangeState")), RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Quest.ChangeState")),
	this, &ThisClass::HandleQuestStateChanged));

	QuestMessageHandlers.Add(FGameplayTag::RequestGameplayTag(FName("Quest.Action.Accept")), FGameplayMessageListenerHandle());
	
}

void ARPGQuestNpc::Interact()
{
	Super::Interact();
	
	const FRPGNpcDataRow* NpcData = RPGHelper::GetNpcData_Safe(this, NpcId);
	if (NpcData == nullptr)
		return;
	
	FStartNpcInteractionMsg Msg;
	Msg.NpcId = NpcId;
	Msg.GivableQuestIds = GivableQuestIds;
	Msg.WaitCompleteQuestIds = WaitCompletionQuestIds;

	RPGMessage::BroadcastMessage(this, Msg, FGameplayTag::RequestGameplayTag(FName("NPCInteraction.StartInteraction")));

	QuestMessageHandlers[FGameplayTag::RequestGameplayTag(FName("Quest.Action.Accept"))] = RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Quest.Action.Accept")),
this, &ThisClass::HandleOnAcceptQuest);
}

void ARPGQuestNpc::Destroyed()
{
	Super::Destroyed();

	for (auto& Pair : QuestMessageHandlers)
	{
		FGameplayMessageListenerHandle& MessageHandler = Pair.Value;
		MessageHandler.Unregister();
	}
}

void ARPGQuestNpc::UpdateGivableQuests()
{
	const FRPGNpcDataRow* NpcDataRow = RPGHelper::GetNpcData_Safe(this, NpcId);
	if (NpcDataRow == nullptr)
		return;
	
	GivableQuestIds.Empty();
	
	if (URPGQuestManager* QuestMgr = RPGHelper::GetQuestManager(this))
	{
		for (const FName& QuestId : NpcDataRow->GivableQuestIds)
		{
			if (const bool IsMet = QuestMgr->CheckMeetQuestPrerequirements(QuestId))
			{
				GivableQuestIds.Add(QuestId);
			}
		}	
	}
	
	UpdateQuestMarkerState();
}

void ARPGQuestNpc::UpdateWaitCompletionQuests()
{
	if (URPGQuestManager* QuestMgr = RPGHelper::GetQuestManager(this))
	{
		QuestMgr->GetWaitCompletionQuestsByNpc(NpcId, WaitCompletionQuestIds);
		UpdateNpcQuestListWidget();
		UpdateQuestMarkerState();
	}
}

void ARPGQuestNpc::UpdateQuestMarkerState() const
{
	if (URPGNpcDescTextWidget* NpcDescTextWidget = Cast<URPGNpcDescTextWidget>(NpcDescWidgetComponent->GetWidget()))
	{
		if (WaitCompletionQuestIds.Num())
		{
			NpcDescTextWidget->UpdateQuestMarker(EQuestMarkerState::WaitComplete);
		}
		else if (GivableQuestIds.Num())
		{
			NpcDescTextWidget->UpdateQuestMarker(EQuestMarkerState::HaveQuest);
		}
		else
		{
			NpcDescTextWidget->UpdateQuestMarker(EQuestMarkerState::None);
		}
	}
}

void ARPGQuestNpc::UpdateNpcQuestListWidget()
{
	FUpdateNpcQuestListMsg Msg;
	Msg.NpcId = NpcId;
	Msg.GivableQuestIds = GivableQuestIds;
	Msg.WaitCompleteQuestIds = WaitCompletionQuestIds;

	RPGMessage::BroadcastMessage(this, Msg, FGameplayTag::RequestGameplayTag("Widget.Quest.UpdateNpcQuestList"));
}

bool ARPGQuestNpc::IsGiverOfQuest(const FName& QuestId) const
{
	const FRPGNpcDataRow* NpcData = RPGHelper::GetNpcData_Safe(this, NpcId);
	if (NpcData == nullptr)
		return false;
	
	for (const FName& Id : NpcData->GivableQuestIds)
	{
		if (Id == QuestId)
		{
			return true;
		}
	}

	return false;
}

bool ARPGQuestNpc::IsCompletionNpc(const FName& QuestId) const
{
	const FRPGQuestDefinition* QuestDef = RPGHelper::GetQuestDefinition_Safe(this, QuestId);
	if (QuestDef == nullptr)
		return false;

	return NpcId == QuestDef->CompletionNPCId;
}


void ARPGQuestNpc::HandleEndInteraction(FGameplayTag Tag, const FEndNpcInteractionMsg& Msg)
{
	Super::HandleEndInteraction(Tag, Msg);

	// 퀘스트 수락 이벤트는 현재 상호작용하고 있는 NPC에만 전달한다.(퀘스트 수락은 상호작용한 NPC에게만 가능하기 때문)
	QuestMessageHandlers[FGameplayTag::RequestGameplayTag(FName("Quest.Action.Accept"))].Unregister();
}

void ARPGQuestNpc::HandleOnAcceptQuest(FGameplayTag Tag, const FOnAcceptQuestMsg& Msg)
{
	if (IsGiverOfQuest(Msg.QuestId))
	{
		UpdateGivableQuests();
		UpdateNpcQuestListWidget();
	}
	
}

void ARPGQuestNpc::HandleOnDropQuest(FGameplayTag Tag, const FOnDropQuestMsg& Msg)
{
	if (IsGiverOfQuest(Msg.QuestId) == true)
	{
		UpdateGivableQuests();
	}

	if (IsCompletionNpc(Msg.QuestId) == true)
	{
		WaitCompletionQuestIds.Remove(Msg.QuestId);
		UpdateQuestMarkerState();
	}
	
}

void ARPGQuestNpc::HandleOnCompleteQuest(FGameplayTag Tag, const FOnCompleteQuestMsg& Msg)
{
	// 해당 퀘스트가 완료
	UpdateGivableQuests();
	if (IsCompletionNpc(Msg.QuestId) == true)
	{
		WaitCompletionQuestIds.Remove(Msg.QuestId);
		UpdateNpcQuestListWidget();
		UpdateQuestMarkerState();
	}
}

void ARPGQuestNpc::HandleQuestStateChanged(FGameplayTag Tag, const FOnQuestStateChangeMsg& Msg)
{
	if (Msg.QuestState == ERPGQuestState::WaitCompletion && IsCompletionNpc(Msg.QuestId))
	{
		WaitCompletionQuestIds.Add(Msg.QuestId);
	}
	else if (Msg.QuestState == ERPGQuestState::Ongoing && IsCompletionNpc(Msg.QuestId))
	{
		// Ongoing State면 전이가 WaitCompletion -> Ongoing 밖에안되므로 (만약 아이템 모으는 퀘스트면 아이템을 모았다가 없어지면 다시 클리어할 수 없는 상태가 된다.)
		WaitCompletionQuestIds.Remove(Msg.QuestId);
	}
	UpdateQuestMarkerState();
}
	

	
	

