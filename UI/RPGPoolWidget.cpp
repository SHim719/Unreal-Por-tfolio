#include "RPGPoolWidget.h"
#include "RPGHelperLibrary.h"
#include "ObjectPool/RPGPoolManager.h"

void URPGPoolWidget::OnExitPool()
{
}

void URPGPoolWidget::OnReturnToPool()
{
}

void URPGPoolWidget::ReturnToPool()
{
	if (auto* PoolManager = RPGHelper::GetPoolManager(this))
	{
		PoolManager->ReturnToPool(PoolKey, this);
	}
}
