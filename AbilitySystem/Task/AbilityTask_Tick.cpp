// 


#include "AbilityTask_Tick.h"

UAbilityTask_Tick::UAbilityTask_Tick(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
}

UAbilityTask_Tick* UAbilityTask_Tick::CreateAbilityTickTask(UGameplayAbility* OwningAbility)
{
	UAbilityTask_Tick* NewTask = NewAbilityTask<UAbilityTask_Tick>(OwningAbility);
	return NewTask;
}

void UAbilityTask_Tick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		TickDelegate.Broadcast(DeltaTime);
	}
}