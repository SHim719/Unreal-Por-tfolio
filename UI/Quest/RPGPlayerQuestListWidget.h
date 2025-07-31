#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "UI/Common/RPGMovableWidget.h"
#include "RPGPlayerQuestListWidget.generated.h"

class UButton;
class URPGQuestInfoWidget;
struct FOnDropQuestMsg;
struct FOnCompleteQuestMsg;
struct FOnAcceptQuestMsg;
class URPGQuestRowWidget;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGPlayerQuestListWidget : public URPGMovableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	void HandleOnAcceptQuest(FGameplayTag Tag, const FOnAcceptQuestMsg& Msg);
	void HandleOnDropQuest(FGameplayTag Tag, const FOnDropQuestMsg& Msg);
	void HandleOnCompleteQuest(FGameplayTag Tag, const FOnCompleteQuestMsg& Msg);

	void AddQuestRow(const FName& QuestId);
	void ShowQuestInfo(const FName& QuestId);
	void RemoveQuestRow(const FName& QuestId);

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPGQuestRowWidget> QuestRowWidgetClass;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Quests;

	UPROPERTY()
	TArray<TObjectPtr<URPGQuestRowWidget>> QuestRowWidgets;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URPGQuestInfoWidget> QuestInfoWidget;
	
};
