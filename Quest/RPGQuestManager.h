// 

#pragma once

#include "CoreMinimal.h"
#include "RPGQuestInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGQuestManager.generated.h"


struct FOnDropQuestMsg;
struct FOnCompleteQuestMsg;
struct FOnAcceptQuestMsg;
class URPGQuestInstance;
struct FRPGQuestDataRow;

UCLASS()
class RPGPROJECT_API URPGQuestManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	bool CheckMeetQuestPrerequirements(const FName& QuestId) const;
	bool IsCompleted(const FName& QuestId) const { return CompletedQuests.Contains(QuestId);}
	bool CheckCanReceiveReward(const FName& QuestId) const;
	void GetWaitCompletionQuestsByNpc(const FName& NpcId, OUT TArray<FName>& WaitCompletionQuests);
	
	URPGQuestInstance* GetQuestInstance(const FName& QuestId) const;
	const TMap<FName, TObjectPtr<URPGQuestInstance>>& GetOngoingQuests() const { return OngoingQuests;}

	void AcceptQuest(const FName& QuestId);
	void DropQuest(const FName& QuestId);
	void CompleteQuest(const FName& QuestId);

private:
	void OnQuestStateChanged(const FName& QuestId, const ERPGQuestState QuestState);
	
private:
	UPROPERTY()
	TMap<FName, TObjectPtr<URPGQuestInstance>> OngoingQuests;
	
	TSet<FName> CompletedQuests;
};
