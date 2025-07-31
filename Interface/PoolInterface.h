// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolInterface.generated.h"

UINTERFACE()
class UPoolInterface : public UInterface
{
	GENERATED_BODY()
};


class RPGPROJECT_API IPoolInterface
{
	GENERATED_BODY()

public:
	virtual void OnExitPool() = 0;
	virtual void OnReturnToPool() = 0;
	
};
