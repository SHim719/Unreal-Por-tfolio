// 

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_Tick.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTickDelegate, float DeltaTime)


UCLASS()
class RPGPROJECT_API UAbilityTask_Tick : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_Tick(const FObjectInitializer& ObjectInitializer);
	
	static UAbilityTask_Tick* CreateAbilityTickTask(UGameplayAbility* OwningAbility);

	FTickDelegate TickDelegate;

private:
	virtual void TickTask(float DeltaTime) override;

};
