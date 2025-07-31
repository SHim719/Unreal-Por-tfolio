// 

#pragma once

#include "CoreMinimal.h"
#include "RPGEffectActor.h"
#include "RPGDefines.h"
#include "RPGSphereAreaEffectActor.generated.h"

class USphereComponent;

UCLASS()
class RPGPROJECT_API ARPGSphereAreaEffectActor : public ARPGEffectActor
{
	GENERATED_BODY()

public:
	ARPGSphereAreaEffectActor();

	
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual bool ApplyEffectToTarget(AActor* OtherActor) override;
	void RemoveEffectFromTarget(UAbilitySystemComponent* ASC, FRPGActiveEffectSpecHandles& ActiveEffectHandles);
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY()
	TMap<int32, FRPGActiveEffectSpecHandles> ActiveEffects;
};
