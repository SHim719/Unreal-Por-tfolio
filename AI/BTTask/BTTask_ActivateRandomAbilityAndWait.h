

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateRandomAbilityAndWait.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API UBTTask_ActivateRandomAbilityAndWait : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UBTTask_ActivateRandomAbilityAndWait();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void OnAbilityEnded(UGameplayAbility* EndedAbility);

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer Tags;

	UPROPERTY(EditAnywhere)
	float WaitTimeAfterAbilityEnd = 0.f;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BTComponent; 
};
