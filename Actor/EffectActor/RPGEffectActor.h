// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "RPGDefines.h"
#include "Actor/RPGPoolActor.h"
#include "RPGEffectActor.generated.h"

USTRUCT()
struct FRPGActiveEffectSpecHandles
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveEffectHandles;
};

UCLASS()
class RPGPROJECT_API ARPGEffectActor : public ARPGPoolActor
{
	GENERATED_BODY() 

public:
	ARPGEffectActor();

	virtual void Initialize();
	void SetEffectSpecHandles(const TArray<FGameplayEffectSpecHandle>& InEffectSpecHandles);
	

protected:
	virtual bool ApplyEffectToTarget(AActor* OtherActor);
	AActor* GetEffectCauser() const;
	
	UPROPERTY()
	TArray<FGameplayEffectSpecHandle> EffectSpecHandles;

	
	
};
