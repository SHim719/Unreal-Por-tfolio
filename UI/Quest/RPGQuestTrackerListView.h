// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "RPGQuestTrackerListView.generated.h"

class UUniformGridSlot;
struct FOnCompleteQuestMsg;
struct FOnDropQuestMsg;
struct FOnAcceptQuestMsg;
class URPGQuestTrackerView;
class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGQuestTrackerListView : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	void HandleOnAcceptQuest(FGameplayTag Tag, const FOnAcceptQuestMsg& Msg);
	void HandleOnDropQuest(FGameplayTag Tag, const FOnDropQuestMsg& Msg);
	void HandleOnCompleteQuest(FGameplayTag Tag, const FOnCompleteQuestMsg& Msg);

	void CreateQuestTrackerView(const FName& QuestId);
	void RemoveQuestTrackerView(const FName& QuestId);
	void RearrangeQuestTrackerViews();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPGQuestTrackerView> QuestTrackerViewClass;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Grid;

	UPROPERTY()
	TArray<TObjectPtr<URPGQuestTrackerView>> QuestTrackerViews;

	UPROPERTY()
	TArray<TObjectPtr<UUniformGridSlot>> QuestTrackerViewSlots;
	
};
