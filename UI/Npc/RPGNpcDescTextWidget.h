

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGNpcDescTextWidget.generated.h"

class UTextBlock;

enum class EQuestMarkerState : uint8
{
	None,
	HaveQuest,
	WaitComplete,
};

UCLASS()
class RPGPROJECT_API URPGNpcDescTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNpcName(const FText& NpcName) const;
	void UpdateQuestMarker(const EQuestMarkerState State) const;
	

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_NpcName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_QuestMarker;
};
