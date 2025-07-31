

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "RPGQuestInstance.generated.h"


struct FQuestTaskSignalMsg;
class URPGQuestTask;

UENUM()
enum class ERPGQuestState
{
	Inactive,
	Ongoing,
	WaitCompletion,
	End,
};

DECLARE_DELEGATE_TwoParams(FOnQuestStateChanged, const FName& QuestId, ERPGQuestState QuestState)
DECLARE_DELEGATE_OneParam(FOnQuestTaskStateChanged, const int32 TaskIndex)


UCLASS()
class RPGPROJECT_API URPGQuestInstance : public UObject
{
	GENERATED_BODY()

public:
	void Init(const FName& QuestId);
	virtual void BeginDestroy() override;

	FName GetRefQuestId() const { return RefQuestId; }
	ERPGQuestState GetQuestState() const { return QuestState;}
	const TArray<TObjectPtr<URPGQuestTask>>& GetCurrentTasks() const { return Tasks;}

	FOnQuestStateChanged		OnQuestStateChanged;
	FOnQuestTaskStateChanged	OnQuestTaskStateChanged;
	
private:
	void ConfirmQuestState();
	void HandleTaskSignalMsg(FGameplayTag Tag, const FQuestTaskSignalMsg& Msg);
	
private:
	FName RefQuestId;
	ERPGQuestState QuestState = ERPGQuestState::Ongoing;
	
	UPROPERTY()
	TArray<TObjectPtr<URPGQuestTask>> Tasks;
	int32 CompletedTaskCount = 0;

	UPROPERTY()
	FGameplayMessageListenerHandle QuestNotifyHandler;
};
