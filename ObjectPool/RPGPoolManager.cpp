#include "RPGPoolManager.h"

#include "Actor/RPGPoolActor.h"
#include "Blueprint/UserWidget.h"
#include "Interface/PoolInterface.h"
#include "UI/RPGPoolWidget.h"


void URPGPoolManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FWorldDelegates::OnPreWorldFinishDestroy.AddUObject(this, &URPGPoolManager::OnWorldDestroyed);
}


void URPGPoolManager::CreateNewPool(const FString& PoolKey, TSubclassOf<UObject> PoolObjectClass, const int32 InitPoolSize,
                                    const int32 MaxPoolSize, const bool bCanGrow, float GrowRate)
{
	if (PoolObjectClass == nullptr || InitPoolSize > MaxPoolSize || InitPoolSize == 0 || MaxPoolSize == 0)
		return;

	if (ObjectPools.Contains(PoolKey))
	{
		ObjectPools.Remove(PoolKey);
	}
	
	FRPGObjectPool NewPool;
	NewPool.MaxPoolSize = MaxPoolSize;
	NewPool.PoolObjectClass = PoolObjectClass;
	NewPool.CurPoolSize = InitPoolSize;
	NewPool.bCanGrow = bCanGrow;
	NewPool.GrowRate = GrowRate;

	NewPool.Objects.Reserve(MaxPoolSize);

	for (int32 i = 0; i < InitPoolSize; ++i)
	{
		NewPool.Objects.Add(CreateNewPoolObject(PoolKey, PoolObjectClass));
	}

	ObjectPools.Add(PoolKey, NewPool);
}

UObject* URPGPoolManager::GetPoolObject(const FString& PoolKey)
{
	FRPGObjectPool* ObjectPool = ObjectPools.Find(PoolKey);
	if (ObjectPool == nullptr)
		return nullptr;

	UObject* PoolObject = nullptr;
	
	// 전부 사용 중이다.
	if (ObjectPool->Objects.IsEmpty()) 
	{
		UObject* NewPoolObject = CreateNewPoolObject(PoolKey, ObjectPool->PoolObjectClass);
		if (NewPoolObject == nullptr)
			return nullptr;
		
		// 최대 풀 사이즈보다 현재 풀 사이즈가 적을 때
		if (ObjectPool->CurPoolSize <= ObjectPool->MaxPoolSize)
		{
			ObjectPool->CurPoolSize++;
			ObjectPool->Objects.Add(NewPoolObject);
		}
		else if (ObjectPool->bCanGrow == true) // Pool의 사이즈를 키울 수 있을 때(재할당)
		{
			ObjectPool->MaxPoolSize = static_cast<int32>(static_cast<float>(ObjectPool->MaxPoolSize) * ObjectPool->GrowRate);
			ObjectPool->CurPoolSize++;
			ObjectPool->Objects.Reserve(ObjectPool->MaxPoolSize);
			ObjectPool->Objects.Add(NewPoolObject);
		}
		
		PoolObject = NewPoolObject;
	}
	else
	{
		PoolObject = ObjectPool->Objects.Pop(EAllowShrinking::No);
	}

	if (PoolObject)
	{
		if (IPoolInterface* PoolInterface = Cast<IPoolInterface>(PoolObject))
		{
			PoolInterface->OnExitPool();
		}
	}
	
	return PoolObject;
}

UObject* URPGPoolManager::CreateNewPoolObject(const FString& PoolKey, TSubclassOf<UObject>& PoolObjectClass) const
{
	UObject* PoolObject = nullptr;
	if (PoolObjectClass->IsChildOf<ARPGPoolActor>())
	{
		if (UWorld* World = GetWorld())
		{
			ARPGPoolActor* PoolActor =  World->SpawnActor<ARPGPoolActor>(PoolObjectClass);
			PoolActor->OnReturnToPool();
			PoolActor->SetPoolKey(PoolKey);
			PoolObject = PoolActor;
		}
	}
	else if (PoolObjectClass->IsChildOf<URPGPoolWidget>())
	{
		URPGPoolWidget* PoolWidget = CreateWidget<URPGPoolWidget>(GetGameInstance(), static_cast<TSubclassOf<UUserWidget>>(PoolObjectClass));
		PoolWidget->SetVisibility(ESlateVisibility::Collapsed);
		PoolWidget->OnReturnToPool();
		PoolWidget->AddToViewport();
		PoolWidget->SetPoolKey(PoolKey);
		PoolObject = PoolWidget;
	}

	return PoolObject;
}

void URPGPoolManager::OnWorldDestroyed(UWorld* World)
{
	ClearAll();
}

void URPGPoolManager::ReturnToPool(const FString& PoolKey, UObject* ReturnObject)
{
	IPoolInterface* PoolInterface = Cast<IPoolInterface>(ReturnObject);
	if (PoolInterface == nullptr)
		return;
	
	if (FRPGObjectPool* ObjectPool = ObjectPools.Find(PoolKey))
	{
		PoolInterface->OnReturnToPool();
		ObjectPool->Objects.Add(ReturnObject);
	}
	else
	{
		ReturnObject->MarkAsGarbage();
	}
	
}

void URPGPoolManager::ClearAll()
{
	for (auto& Pool : ObjectPools)
	{
		Pool.Value.PoolObjectClass = nullptr;
		Pool.Value.Objects.Empty();
	}

	ObjectPools.Empty();
}
	
	
	




