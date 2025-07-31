// 

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGDialogueManager.generated.h"


class URPGDialogueData;

UCLASS()
class RPGPROJECT_API URPGDialogueManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void StartDialogue(FText InCurrentNpcNameText, URPGDialogueData* DialogueData);
	void UpdateDialogueLine();

private:
	void UpdateDialogueBoxWidget();

private:
	UPROPERTY()
	TObjectPtr<URPGDialogueData> CurrentDialogue;

	int32 CurrentDialogueIndex = -1;

	FText CurrentNpcNameText;
};
