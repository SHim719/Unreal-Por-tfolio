#include "RPGQuestManager.h"

#include "RPGHelperLibrary.h"
#include "RPGQuestInstance.h"
#include "Character/RPGPlayerCharacter.h"
#include "Components/RPGInventoryComponent.h"
#include "Data/Quest/RPGQuestData.h"
#include "Data/Quest/RPGQuestPrerequirement.h"
#include "Kismet/GameplayStatics.h"
#include "Message/RPGGameMessage.h"



bool URPGQuestManager::CheckMeetQuestPrerequirements(const FName& QuestId) const
{
	// 이미 클리어 한 퀘스트거나 진행중이면 false 반환.
	if (CompletedQuests.Contains(QuestId) == true || OngoingQuests.Contains(QuestId) == true)
		return false;
	
	if (const FRPGQuestDefinition* QuestDef = RPGHelper::GetQuestDefinition_Safe(GetGameInstance(), QuestId))
	{
		ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetGameInstance(), 0));
		if (PlayerCharacter == nullptr)
			return false;
		
		for (URPGQuestPrerequirement* QuestPrerequirement : QuestDef->QuestPrerequirements)
		{
			if (QuestPrerequirement->IsMetPrerequirement(PlayerCharacter) == false)
				return false;
		}
	}
	
	return true;
}

bool URPGQuestManager::CheckCanReceiveReward(const FName& QuestId) const
{
	const FRPGQuestDefinition* QuestDef = RPGHelper::GetQuestDefinition_Safe(this, QuestId);
	if (QuestDef == nullptr)
		return false;

	ARPGPlayerState* PlayerState = RPGHelper::GetPlayerState_Safe(this);
	if (PlayerState == nullptr)
		return false;

	URPGInventoryComponent* InventoryComponent = PlayerState->FindComponentByClass<URPGInventoryComponent>();
	if (InventoryComponent == nullptr)
		return false;
	
	for (const auto& Reward : QuestDef->QuestRewards)
	{
		if (InventoryComponent->CanInsertItem(Reward.RewardItemId, Reward.RewardCount) == false)
			return false;
	}

	return true;
}

void URPGQuestManager::GetWaitCompletionQuestsByNpc(const FName& NpcId, TArray<FName>& WaitCompletionQuests)
{
	for (auto& Pair : OngoingQuests)
	{
	  	URPGQuestInstance* QuestInstance = Pair.Value;
	  	if (QuestInstance == nullptr)
			continue;
		
		const FRPGQuestDefinition* QuestDef = RPGHelper::GetQuestDefinition_Safe(this, QuestInstance->GetRefQuestId());
		if (QuestDef == nullptr)
			continue;
		
	  	ERPGQuestState QuestState = QuestInstance->GetQuestState();
	  	if (QuestDef->CompletionNPCId == NpcId && QuestState == ERPGQuestState::WaitCompletion)
	  	{
			WaitCompletionQuests.AddUnique(QuestInstance->GetRefQuestId());
	  	}
	}

}

URPGQuestInstance* URPGQuestManager::GetQuestInstance(const FName& QuestId) const
{
	auto QuestInstancePtr = OngoingQuests.Find(QuestId);
	if (QuestInstancePtr == nullptr)
		return nullptr;

	return *QuestInstancePtr;
}


void URPGQuestManager::OnQuestStateChanged(const FName& QuestId, const ERPGQuestState QuestState)
{
	FOnQuestStateChangeMsg Msg;
	Msg.QuestId = QuestId;
	Msg.QuestState = QuestState;
	RPGMessage::BroadcastMessage(this, Msg, FGameplayTag::RequestGameplayTag(FName("Quest.ChangeState")));
}


void URPGQuestManager::AcceptQuest(const FName& QuestId)
{
	const FRPGQuestDefinition* QuestDef = RPGHelper::GetQuestDefinition_Safe(this, QuestId);
	if (QuestDef == nullptr)
		return;

	if (URPGQuestInstance* QuestInstance = NewObject<URPGQuestInstance>(this))
	{
		QuestInstance->OnQuestStateChanged.BindUObject(this, &ThisClass::OnQuestStateChanged);
		OngoingQuests.Add(QuestId, QuestInstance);
		QuestInstance->Init(QuestId); // 초기화할 때, 현재 Task 검사도 하기때문에 나중에 호출해준다.
	}

	FOnAcceptQuestMsg Msg;
	Msg.QuestId = QuestId; 
	RPGMessage::BroadcastMessage(this, Msg, FGameplayTag::RequestGameplayTag(FName("Quest.Action.Accept")));	
	
}

void URPGQuestManager::DropQuest(const FName& QuestId)
{
	OngoingQuests.Remove(QuestId);

	FOnDropQuestMsg Msg;
	Msg.QuestId = QuestId;
	RPGMessage::BroadcastMessage(this, Msg, FGameplayTag::RequestGameplayTag(FName("Quest.Action.Drop")));
}

void URPGQuestManager::CompleteQuest(const FName& QuestId)
{
	const FRPGQuestDefinition* QuestDef = RPGHelper::GetQuestDefinition_Safe(this, QuestId);
	if (QuestDef == nullptr)
		return;
	
	ARPGPlayerState* PlayerState = RPGHelper::GetPlayerState_Safe(this);
	if (PlayerState == nullptr)
		return;

	URPGInventoryComponent* InventoryComponent = PlayerState->FindComponentByClass<URPGInventoryComponent>();
	if (InventoryComponent == nullptr)
		return;
	
	for (const auto& Reward : QuestDef->QuestRewards)
	{
		InventoryComponent->InsertItem(Reward.RewardItemId, Reward.RewardCount);
	}
	
	OngoingQuests.Remove(QuestId);
	CompletedQuests.Add(QuestId);

	FOnCompleteQuestMsg Msg;
	Msg.QuestId = QuestId; 
	RPGMessage::BroadcastMessage(this, Msg, FGameplayTag::RequestGameplayTag(FName("Quest.Action.Complete")));	
}





