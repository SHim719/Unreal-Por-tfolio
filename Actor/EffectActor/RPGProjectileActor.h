// 

#pragma once

#include "CoreMinimal.h"
#include "RPGEffectActor.h"
#include "RPGProjectileActor.generated.h"


class UProjectileMovementComponent;
class UNiagaraSystem;
class USphereComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPGPROJECT_API ARPGProjectileActor : public ARPGEffectActor
{
	GENERATED_BODY()
	
public:
	ARPGProjectileActor();

protected:
	virtual void BeginPlay() override;
	virtual void OnExitPool() override;
	virtual void ReturnToPool() override;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintImplementableEvent)
	void HitImpact();
public:
	void ResetVelocity();
	
protected:
	FTimerHandle LifeSpanTimerHandle;
	int32 CurrentHitCount = 0;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 10.f;
	
	UPROPERTY(EditDefaultsOnly)
	int32 HitCount = 1;
	
};
