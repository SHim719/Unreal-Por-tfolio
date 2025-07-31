// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGQuestRowWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnClickedQuestRowButton, const FName& QuestId)

class UTextBlock;
class UButton;

UCLASS()
class RPGPROJECT_API URPGQuestRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	void SetRefQuestId(const FName& QuestId);
	FName GetRefQuestId() const { return RefQuestId;}

	FOnClickedQuestRowButton OnClickedQuestRowButton;

private:
	UFUNCTION()
	void OnClickedButton();

private:
	FName RefQuestId;
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Quest;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_QuestTitle;
 };
