// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGRadialIndicator.generated.h"

UCLASS()
class RPGPROJECT_API ARPGRadialIndicator : public AActor
{
	GENERATED_BODY()

public:
	ARPGRadialIndicator();

	UFUNCTION(BlueprintNativeEvent)
	void Initialize(const float Radius, const float Angle, const float fillAmount);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateFillAmount(const float FillAmount);
protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UDecalComponent> DecalComponent;
};
