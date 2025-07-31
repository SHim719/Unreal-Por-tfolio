

#pragma once

#include "CoreMinimal.h"
#include "UI/RPGPoolWidget.h"
#include "RPGDamageText.generated.h"

class UTextBlock;

UCLASS()
class RPGPROJECT_API URPGDamageText : public URPGPoolWidget
{
	GENERATED_BODY()

public:
	void SetDamage(const float Damage, const bool bIsCritical);

private:
	void OnAnimationSequenceFinished(UUMGSequencePlayer& Player);

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Damage;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> DamageAnimation;

	UPROPERTY(EditDefaultsOnly)
	FSlateColor NormalColor;
	
	UPROPERTY(EditDefaultsOnly)
	FSlateColor CriticalColor;
	
};
