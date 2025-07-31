// 

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RPGAbility_PlayerMove.generated.h"

class UAbilityTask_Tick;
class ARPGPlayerController;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGAbility_PlayerMove : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	void Tick(float DeltaTime);
	void UpdateDestination();
	void MoveToDestination();
	
private:
	UPROPERTY(EditAnywhere)
	float ShortPressThreshold = 1.f;

	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 10.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> FXCursor;

private:
	UPROPERTY()
	TObjectPtr<UAbilityTask_Tick> TickTask;

	FVector CachedLocation;
	bool bMoveToDestination = false;
	bool bCountPressedTime = false;
	float PressedTime = 0.f;
};
