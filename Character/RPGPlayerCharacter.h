

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Character/RPGCharacterBase.h"
#include "RPGPlayerCharacter.generated.h"


class ARPGEquipmentActor;
struct FOnPlayerChangeEquipmentMsg;
class USplineComponent;
class UNiagaraSystem;
class URPGInteractionComponent;
class USphereComponent;
class URPGMovementHandlerComponent;
class USpringArmComponent;
class UCameraComponent;
class ARPGPlayerCaptureCharacter;

UCLASS()
class RPGPROJECT_API ARPGPlayerCharacter : public ARPGCharacterBase
{
	GENERATED_BODY()

public:
	ARPGPlayerCharacter();
	virtual void BeginPlay() override;
	void Initialize();
protected:

	virtual void InitAbilityInfo() override;
	virtual void Die() override;
	
	void OnLevelChanged(const int32 NewLevel, bool bLevelUp) const;
	void HandleOnPlayerEquipmentChange(FGameplayTag, const FOnPlayerChangeEquipmentMsg& Msg);

	UFUNCTION()
	void OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY()
	TObjectPtr<ARPGEquipmentActor> WeaponActor;
	

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARPGPlayerCaptureCharacter>		PlayerCaptureCharacterClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem>					LevelUpVfx;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase>						LevelUpSound;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent>				CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent>				SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent>				SplineComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent>				InteractionSphereComponent;

	UPROPERTY()
	TObjectPtr<URPGInteractionComponent>		InteractionComponent;
	

};
