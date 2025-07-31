// 

#pragma once
#include "Data/Dialogue/RPGDialogueData.h"


#include "RPGQuestData.generated.h"


class URPGQuestPrerequirement;
class URPGQuestTask;

USTRUCT()
struct FRPGQuestReward
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	FName RewardItemId;

	UPROPERTY(EditDefaultsOnly)
	int32 RewardCount = 0;
};


USTRUCT()
struct FRPGQuestDefinition
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	FText QuestTitle;

	UPROPERTY(EditDefaultsOnly)
	FText QuestDescription;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URPGDialogueData> StartDialogue;

	UPROPERTY(EditDefaultsOnly)
	FName CompletionNPCId;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URPGDialogueData> CompletionDialogue;

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<URPGQuestTask>> QuestTasks;

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "QuestTasks.IsEmpty()", EditConditionHides))
	FText OverrideTaskDesc;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FRPGQuestReward> QuestRewards;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<URPGQuestPrerequirement>> QuestPrerequirements;
};

UCLASS(Const)
class URPGQuestData : public UDataAsset
{
	GENERATED_BODY()

public:
	URPGQuestData();
	
public:
	//	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
	//
	//#if WITH_EDITOR
	//	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	//#endif
	//	
public:
	const FRPGQuestDefinition* FindQuestDefinitionByName(const FName& QuestName) const;

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FRPGQuestDefinition> NameToQuest;
	
};

