
#include "AnimInstance/RPGPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URPGPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (const ACharacter* Owner = Cast<ACharacter>(TryGetPawnOwner()))
	{
		MovementComp = Owner->GetCharacterMovement();
	}
}

void URPGPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (MovementComp)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComp->Velocity);
	}

}
