#include "RPGDialogueAction.h"

#include "Message/RPGGameMessage.h"



void URPGDialogueAction_ShowQuestInfo::Execute(UObject* WorldContextObject)
{
	FShowNpcQuestInfoMsg ShowQuestInfoMsg;
	ShowQuestInfoMsg.QuestId = QuestId;
	
	RPGMessage::BroadcastMessage(WorldContextObject, ShowQuestInfoMsg, FGameplayTag::RequestGameplayTag(FName("Widget.Quest.ShowNpcQuestInfo")));
}
