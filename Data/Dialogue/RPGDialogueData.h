// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPGDialogueData.generated.h"

class URPGDialogueAction;

USTRUCT()
struct FRPGDialogueLine
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	bool bIsPlayerSpeak;
	
	UPROPERTY(EditDefaultsOnly, meta=(MultiLine = "true"))
	FText Dialogue;

	UPROPERTY(EditDefaultsOnly)
	int32 NextDialogueIndex;

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<URPGDialogueAction>> DialogueActions;
};


UCLASS()
class URPGDialogueData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FRPGDialogueLine> Dialogues;
};
