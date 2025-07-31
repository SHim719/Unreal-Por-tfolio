// 


#include "RPGPoolActor.h"

#include "RPGHelperLibrary.h"
#include "ObjectPool/RPGPoolManager.h"


void ARPGPoolActor::OnExitPool()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
}

void ARPGPoolActor::OnReturnToPool()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void ARPGPoolActor::ReturnToPool()
{
	if (auto* PoolManager = RPGHelper::GetPoolManager(this))
	{
		PoolManager->ReturnToPool(PoolKey, this);
	}
}
