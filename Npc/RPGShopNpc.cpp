// 


#include "RPGShopNpc.h"

#include "RPGHelperLibrary.h"
#include "Data/Npc/RPGNpcData.h"
#include "Message/RPGGameMessage.h"


ARPGShopNpc::ARPGShopNpc()
{
}

void ARPGShopNpc::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGShopNpc::Interact()
{
	Super::Interact();

	const FRPGNpcDataRow* NpcData = RPGHelper::GetNpcData_Safe(this, NpcId);
	if (NpcData == nullptr)
		return;
	
	FStartNpcInteractionMsg Msg;
	Msg.NpcId = NpcId;

	RPGMessage::BroadcastMessage(this, Msg, FGameplayTag::RequestGameplayTag(FName("NPCInteraction.StartInteraction")));
}
