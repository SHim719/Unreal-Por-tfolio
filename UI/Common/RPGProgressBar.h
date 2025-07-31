

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGProgressBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGProgressBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateProgressBarPercentage();
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;

	UPROPERTY(BlueprintReadWrite)
	float MaxValue = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float CurrentValue = 0.f;
	
};
