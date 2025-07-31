#include "RPGQuestPrerequirement.h"

#include "RPGHelperLibrary.h"
#include "Character/RPGPlayerCharacter.h"
#include "Quest/RPGQuestManager.h"

bool URPGQuestPrerequirement_Level::IsMetPrerequirement(ARPGPlayerCharacter* PlayerCharacter) const
{
	if (ARPGPlayerState* PlayerState = RPGHelper::GetPlayerState_Safe(PlayerCharacter))
	{
		return PlayerState->GetPlayerLevel() >= Level;
	}
	
	return false;
}

bool URPGQuestPrerequirement_Quest::IsMetPrerequirement(ARPGPlayerCharacter* PlayerCharacter) const
{
	URPGQuestManager* QuestMgr = RPGHelper::GetQuestManager(PlayerCharacter);
	if (QuestMgr == nullptr)
		return false;

	for (const FName& QuestId : CompletedQuests)
	{
		if (QuestMgr->IsCompleted(QuestId) == false)
			return false;
	}

	return true;
}
