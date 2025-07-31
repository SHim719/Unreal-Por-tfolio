

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPGPlayerAnimInstance.generated.h"


class UCharacterMovementComponent;

UCLASS()
class RPGPROJECT_API URPGPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float GroundSpeed;
};
