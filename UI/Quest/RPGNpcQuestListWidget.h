// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "RPGNpcQuestListWidget.generated.h"

struct FUpdateNpcQuestListMsg;
struct FShowNpcQuestInfoMsg;
struct FOnCompleteQuestMsg;
struct FOnAcceptQuestMsg;
class URPGQuestRowWidget;
class URPGQuestInfoWidget;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGNpcQuestListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	void InitializeQuestList(const FName& NpcId, const TArray<FName>& GivableQuestIds, const TArray<FName>& WaitCompleteQuestIds);
	
private:
	void HandleOnAcceptQuest(FGameplayTag Tag, const FOnAcceptQuestMsg& Msg);
	void HandleOnCompleteQuest(FGameplayTag Tag, const FOnCompleteQuestMsg& Msg);
	void HandleShowQuestInfoMsg(FGameplayTag, const FShowNpcQuestInfoMsg& Msg);
	
	
	void StartQuestDialogue(const FName& QuestId);
	void ShowQuestInfoWidget(const FName& QuestId);

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPGQuestRowWidget> QuestRowWidgetClass;


private:
	FName CurrentNpcId;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_CanAccept;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_CanComplete;

	UPROPERTY()
	TArray<TObjectPtr<URPGQuestRowWidget>> QuestRowWidgets_CanAccept;
	UPROPERTY()
	TArray<TObjectPtr<URPGQuestRowWidget>> QuestRowWidgets_CanComplete;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URPGQuestInfoWidget> QuestInfoWidget;
	
};
