// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Message/RPGGameMessage.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGNpcInteractionManager.generated.h"

class URPGShopWidget;
class URPGNpcQuestListWidget;
struct FStartNpcInteractionMsg;
struct FGameplayTag;

class UButton;
class URPGDialogueBox;

UCLASS()
class RPGPROJECT_API URPGNpcInteractionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
private:
	void Init(UWorld* World, const UWorld::InitializationValues);
	
	UFUNCTION()
	void OnClicked_ExitButton();

	void HandleStartInteraction(FGameplayTag Tag, const FStartNpcInteractionMsg& Msg);
	void HandleUpdateQuestListMsg(FGameplayTag, const FUpdateNpcQuestListMsg& Msg);
private:
	FName CurInteractionNpcId;
	
	UPROPERTY()
	TObjectPtr<URPGDialogueBox> DialogueBox;

	UPROPERTY()
	TObjectPtr<URPGNpcQuestListWidget> NpcQuestList;

	UPROPERTY()
	TObjectPtr<URPGShopWidget> NpcShopWidget;
	
	
};
