


#include "AI/BTServices/BTService_DetectAndTrackTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/RPGCharacterBase.h"
#include "Kismet/GameplayStatics.h"

UBTService_DetectAndTrackTarget::UBTService_DetectAndTrackTarget()
{
	NodeName = "Detect and Track Target";
}

void UBTService_DetectAndTrackTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return;

	ARPGCharacterBase* ControlledPawn = Cast<ARPGCharacterBase>(AIController->GetPawn());
	if (ControlledPawn == nullptr)
		return;
	
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
		return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AActor* FoundTarget = nullptr;

	if (PlayerPawn)
	{
		float DistanceToPlayer = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerPawn->GetActorLocation());

		if (DistanceToPlayer <= DetectionRadius)
		{
			bool bHasLineOfSight = true;
			if (bCheckLineOfSight)
			{
				bHasLineOfSight = AIController->LineOfSightTo(PlayerPawn);
				// DrawDebugLine(GetWorld(), ControlledPawn->GetActorLocation(), PlayerPawn->GetActorLocation(), bHasLineOfSight ? FColor::Green : FColor::Red, false, 0.1f, 0, 5.f);
			}

			if (bHasLineOfSight)
			{
				FoundTarget = PlayerPawn;
			}
		}
	}
	
	
	if (FoundTarget)
	{
		Blackboard->SetValueAsObject(TargetActorKey.SelectedKeyName, FoundTarget);
		float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), FoundTarget->GetActorLocation());
		Blackboard->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, Distance);
	}
	else
	{
		Blackboard->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, FLT_MAX);
		Blackboard->ClearValue(TargetActorKey.SelectedKeyName);
	}

	
}
