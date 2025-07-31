#include "RPGAbility_PlayerMove.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "RPGDefines.h"
#include "AbilitySystem/Task/AbilityTask_Tick.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

void URPGAbility_PlayerMove::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	TickTask = UAbilityTask_Tick::CreateAbilityTickTask(this);
	if (TickTask)
	{
		TickTask->TickDelegate.AddUObject(this, &ThisClass::Tick);
		TickTask->ReadyForActivation();
	}
	
	
}

void URPGAbility_PlayerMove::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	bMoveToDestination = false;
	bCountPressedTime = false;
	PressedTime = 0.f;
	
}

void URPGAbility_PlayerMove::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	bCountPressedTime = true;
}

void URPGAbility_PlayerMove::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	bCountPressedTime = false;
	PressedTime = 0.f;

	if (PressedTime < ShortPressThreshold)
	{
		UpdateDestination();
	}
}

void URPGAbility_PlayerMove::Tick(float DeltaTime)
{
	if (bCountPressedTime == true)
	{
		PressedTime += DeltaTime;
	}

	if (bMoveToDestination == true)
	{
		MoveToDestination();
	}
}

void URPGAbility_PlayerMove::UpdateDestination()
{
	auto* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC == nullptr)
		return;
	
	FHitResult HitResult;
	if (PC->GetHitResultUnderCursor(ECC_Terrain, false, HitResult))
	{
		FVector DestLocation = HitResult.Location;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, DestLocation, FRotator::ZeroRotator, FVector(0.5f, 0.5f, 0.5f), true, true, ENCPoolMethod::None, true);
		bMoveToDestination = true;

		USplineComponent* Spline = GetOwningActorFromActorInfo()->FindComponentByClass<USplineComponent>();
		if (Spline == nullptr)
			return;

		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, GetOwningActorFromActorInfo()->GetActorLocation(), DestLocation))
		{
			Spline->ClearSplinePoints();
			for (const FVector& Point : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World);
			}
			
			if (NavPath->PathPoints.Num())
				CachedLocation = NavPath->PathPoints.Last();
		}
	}
	
}

void URPGAbility_PlayerMove::MoveToDestination()
{
	APawn* ControlledPawn = Cast<APawn>(GetOwningActorFromActorInfo());
	if (ControlledPawn == nullptr)
		return;
	
	USplineComponent* Spline = ControlledPawn->FindComponentByClass<USplineComponent>();
	if (Spline == nullptr)
		return;
	
	const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
	const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
	ControlledPawn->AddMovementInput(Direction);
 
	const float DistanceToDestination = (LocationOnSpline - CachedLocation).Length();
	if (DistanceToDestination <= AcceptanceRadius)
	{
		bMoveToDestination = false;
		K2_EndAbility();
	}
}
