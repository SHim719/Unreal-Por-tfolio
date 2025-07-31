#include "AI/BTTask/BTTask_ActivateRandomAbilityAndWait.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

UBTTask_ActivateRandomAbilityAndWait::UBTTask_ActivateRandomAbilityAndWait()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_ActivateRandomAbilityAndWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Tags.IsEmpty())
		return EBTNodeResult::Failed;

	const int32 RandIndex = FMath::RandRange(0, Tags.Num() - 1);
	const FGameplayTag RandTag = Tags.GetByIndex(RandIndex);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;

	APawn* ControlledPawn = AIController->GetPawn();
	if (ControlledPawn == nullptr)
		return EBTNodeResult::Failed;
	
	auto* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn);
	if (ASC == nullptr)
		return EBTNodeResult::Failed;

	bool bResult = ASC->TryActivateAbilitiesByTag(RandTag.GetSingleTagContainer());
	if (bResult == false)
		return EBTNodeResult::Failed;

	ASC->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnAbilityEnded);

	BTComponent = &OwnerComp;
	
	return EBTNodeResult::InProgress;
}

void UBTTask_ActivateRandomAbilityAndWait::OnAbilityEnded(UGameplayAbility* EndedAbility)
{
	if (auto* ASC = EndedAbility->GetAbilitySystemComponentFromActorInfo())
	{
		ASC->AbilityEndedCallbacks.RemoveAll(this);
		if (WaitTimeAfterAbilityEnd == 0.f)
		{
			FinishLatentTask(*BTComponent, EBTNodeResult::Succeeded);
			BTComponent = nullptr;
		}
		else
		{
			FTimerHandle TimerHandle;
			ASC->GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
			{
				FinishLatentTask(*BTComponent, EBTNodeResult::Succeeded);
				BTComponent = nullptr;
			}), WaitTimeAfterAbilityEnd, false);
		}
	}
	
}

