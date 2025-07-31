// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PoolInterface.h"
#include "RPGPoolActor.generated.h"

UCLASS()
class RPGPROJECT_API ARPGPoolActor : public AActor, public IPoolInterface
{
	GENERATED_BODY()

public:
	virtual void OnExitPool() override;
	virtual void OnReturnToPool() override;

	void SetPoolKey(const FString& InPoolKey) { PoolKey = InPoolKey;}

protected:
	virtual void ReturnToPool();

protected:
	FString PoolKey;

};
