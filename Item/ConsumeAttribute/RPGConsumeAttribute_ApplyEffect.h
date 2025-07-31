// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RPGConsumeAttribute.h"
#include "RPGConsumeAttribute_ApplyEffect.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGConsumeAttribute_ApplyEffect : public URPGConsumeAttribute
{
	GENERATED_BODY()

public:
	virtual bool Consume(AActor* Player) override;
	virtual FText GetTooltipText(const UObject* WorldContextObject) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, int32> SetByCallerValues;
	
	UPROPERTY(EditDefaultsOnly)
	bool bHasDuration = false;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bHasDuration == true", EditConditionHides))
	float Duration = 0.f;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bHasDuration == true", EditConditionHides))
	float Period = 0.f;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bHasDuration == true", EditConditionHides))
	int32 StackCount = 1;
};
