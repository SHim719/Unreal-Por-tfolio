

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectAndTrackTarget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UBTService_DetectAndTrackTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_DetectAndTrackTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DistanceToTargetKey;
	
	UPROPERTY(EditAnywhere)
	float DetectionRadius = 1000.0f;
	
	UPROPERTY(EditAnywhere)
	bool bCheckLineOfSight = true;
};
