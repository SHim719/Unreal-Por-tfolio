// 

#pragma once

#include "CoreMinimal.h"
#include "RPGEffectActor.h"
#include "RPGExplosionActor.generated.h"

class UNiagaraComponent;
class ARPGRadialIndicator;

UCLASS()
class RPGPROJECT_API ARPGExplosionActor : public ARPGEffectActor
{
	GENERATED_BODY()

public:
	ARPGExplosionActor();

	void Initialize();
	virtual void OnReturnToPool() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void ShowExplodeEffect();

protected:
	void QueryAffectRange();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARPGRadialIndicator> RadialIndicatorClass;
	
	UPROPERTY(EditDefaultsOnly)
	float ExplosionDelay = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float AffectAngle = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float AffectRadius = 0.f;

	UPROPERTY()
	TObjectPtr<ARPGRadialIndicator> RadialIndicator;
	
	float CurTime = 0.f;
};
