// 

#pragma once

#include "CoreMinimal.h"
#include "RPGMovableWidget.h"
#include "RPGConfirmWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DELEGATE(FConfirmButtonClicked)
DECLARE_DELEGATE(FCancelButtonClicked)


UCLASS()
class RPGPROJECT_API URPGConfirmWidget : public URPGMovableWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	void SetDescriptionText(const FText& Text);
	
	FConfirmButtonClicked OnConfirmButtonClicked;
	FCancelButtonClicked OnCancelButtonClicked;

private:
	UFUNCTION()
	void OnClicked_ConfirmButton();

	UFUNCTION()
	void OnClicked_CancelButton();
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Description;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Confirm;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Cancel;
	
};
