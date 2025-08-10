// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGFadeScreen.generated.h"

enum class EFadeType
{
	FadeIn,
	FadeOut,
};

DECLARE_DELEGATE(FOnFadeEnded)

UCLASS()
class RPGPROJECT_API URPGFadeScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void StartFade(const EFadeType FadeType);


	FOnFadeEnded	OnFadeEndedDelegate;
private:
	void OnAnimationSequenceFinished(UUMGSequencePlayer& Player);
	
private:
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeIn;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;
};
