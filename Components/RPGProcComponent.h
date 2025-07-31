// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RPGProcComponent.generated.h"


class UAbilitySystemComponent;
struct FGameplayEventData;
class URPGItemInstance;
struct FRPGProcEffectInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPGPROJECT_API URPGProcComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URPGProcComponent();
	void Init();

	void AddProcEffect(const FRPGProcEffectInstance& ProcEffectInstance);
	void RemoveProcEffect(const FRPGProcEffectInstance& RemoveProcEffect);

private:
	void HandleGameplayEvent(const FGameplayEventData* Payload);

	
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerASC;
	
	UPROPERTY()
	TArray<FRPGProcEffectInstance> ProcEffectInstances;
	
	TMap<FGameplayTag, FDelegateHandle> ProcEventHandles;
};
