

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGIndexedButton.generated.h"

class UButton;
DECLARE_DELEGATE_OneParam(FOnButtonClickedHandler, int32)


UCLASS()
class RPGPROJECT_API URPGIndexedButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	void SetIndex(const int32 InIndex) { ButtonIndex = InIndex;}
	
	FOnButtonClickedHandler OnButtonClicked;

private:
	UFUNCTION()
	void HandleButtonClicked();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Button;
	
	int32 ButtonIndex = 0;
};


