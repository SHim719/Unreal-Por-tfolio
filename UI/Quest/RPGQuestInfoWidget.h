// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Quest/RPGQuestData.h"
#include "Quest/RPGQuestInstance.h"
#include "RPGQuestInfoWidget.generated.h"

struct FRPGQuestReward;
struct FRPGQuestDefinition;
class URPGQuestTaskTextWidget;
class UVerticalBox;
class UButton;
class URPGGridSlots;
class UTextBlock;


USTRUCT()
struct FQuestButtonStateInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	ERPGQuestState QuestState;

	UPROPERTY(EditDefaultsOnly)
	FText ButtonText;
};

UCLASS()
class RPGPROJECT_API URPGQuestInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	void UpdateQuestInfo(const FName& QuestId, const ERPGQuestState QuestState);

private:
	void UpdateQuestTaskTexts(const FName& QuestId, const ERPGQuestState QuestState);
	void UpdateQuestTaskTexts(const TArray<URPGQuestTask*>& QuestTasks);
	void UpdateRewardInfo(const TArray<FRPGQuestReward>& QuestRewards);
	
	UFUNCTION()
	void OnClicked_Accept();
	UFUNCTION()
	void OnClicked_Drop();
	UFUNCTION()
	void OnClicked_Complete();

	UFUNCTION()
	void OnClicked_Exit();

private:
	FName RefQuestId;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TMap<ERPGQuestState, FText> QuestButtonStateInfos;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_QuestTitle;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Tasks;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Description;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URPGGridSlots> Grid_Rewards;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_AcceptOrDropOrComplete;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_AcceptOrDropOrComplete;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Exit;
	
};
