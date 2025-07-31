// 

#pragma once

#include "CoreMinimal.h"
#include "RPGNpcBase.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "RPGQuestNpc.generated.h"

struct FOnCompleteQuestMsg;
struct FOnQuestStateChangeMsg;
struct FOnDropQuestMsg;
struct FOnAcceptQuestMsg;

UCLASS()
class RPGPROJECT_API ARPGQuestNpc : public ARPGNpcBase
{
	GENERATED_BODY()
	
public:
	ARPGQuestNpc();
	virtual void BeginPlay() override;
	virtual void Interact() override;
	virtual void Destroyed() override;
	
protected:
	void UpdateGivableQuests();
	void UpdateWaitCompletionQuests();
	void UpdateQuestMarkerState() const;
	void UpdateNpcQuestListWidget();
	bool IsGiverOfQuest(const FName& QuestId) const;
	bool IsCompletionNpc(const FName& QuestId) const;

	virtual void HandleEndInteraction(FGameplayTag Tag, const FEndNpcInteractionMsg& Msg) override;
	void HandleOnAcceptQuest(FGameplayTag Tag, const FOnAcceptQuestMsg& Msg);
	void HandleOnDropQuest(FGameplayTag Tag, const FOnDropQuestMsg& Msg);
	void HandleOnCompleteQuest(FGameplayTag Tag, const FOnCompleteQuestMsg& Msg);
	void HandleQuestStateChanged(FGameplayTag Tag, const FOnQuestStateChangeMsg& Msg);

protected:
	TArray<FName> GivableQuestIds;
	TArray<FName> WaitCompletionQuestIds;

	UPROPERTY()
	TMap<FGameplayTag, FGameplayMessageListenerHandle> QuestMessageHandlers;
};
