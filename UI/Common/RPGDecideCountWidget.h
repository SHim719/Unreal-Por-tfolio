// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGDecideCountWidget.generated.h"

class USpinBox;
class UButton;


DECLARE_DELEGATE_OneParam(FOnDecideCountDelegate, const int32 Count)
DECLARE_DELEGATE(FOnCancelDelegate)

UCLASS()
class RPGPROJECT_API URPGDecideCountWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void Init(const int32 MaxValue);


	FOnDecideCountDelegate OnDecideCountDelegate;
	FOnCancelDelegate OnCancelDelegate;
private:
	UFUNCTION()
	void OnConfirmButtonClicked();
	UFUNCTION()
	void OnCancelButtonClicked();

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USpinBox> SpinBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Confirm;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Cancel;
	
};


