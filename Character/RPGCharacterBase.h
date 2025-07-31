

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "RPGCharacterBase.generated.h"

struct FGameplayEventData;
class UMotionWarpingComponent;
class URPGAttributeSet;
class URPGAbilitySystemComponent;

UCLASS()
class RPGPROJECT_API ARPGCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ARPGCharacterBase();
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void OnHit(const FGameplayEventData* Payload);
	virtual void Die();
	
	void ShowDamageNumberText(const float Damage, const bool bIsCritical) const;

	
protected:
	virtual void InitAbilityInfo();

	

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent>					DamageNumberPositionComponent;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<URPGAbilitySystemComponent>		AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<URPGAttributeSet>				AttributeSet;

	UPROPERTY(EditAnywhere)						
	TObjectPtr<UMotionWarpingComponent>			MotionWarpingComponent;
};
