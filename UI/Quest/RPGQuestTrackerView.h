// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGQuestTrackerView.generated.h"


class URPGQuestInstance;
class UTextBlock;

UCLASS()
class RPGPROJECT_API URPGQuestTrackerView : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(const FName& QuestId);
	
	FName GetRefQuestId() const { return RefQuestId; }

private:
	void UpdateTaskText(const int32 TaskIndex);
	
private:
	FName			RefQuestId;

	UPROPERTY()
	TObjectPtr<URPGQuestInstance> RefQuestInstance;
	
	TArray<FText>	TaskTexts;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_QuestTitle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Tasks;
	
};
