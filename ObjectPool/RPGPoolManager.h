// 

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGPoolManager.generated.h"


USTRUCT()
struct FRPGObjectPool
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<UObject>> Objects;

	UPROPERTY()
	TSubclassOf<UObject> PoolObjectClass;
	
	int32 MaxPoolSize = 50;
	int32 CurPoolSize = 0;

	float GrowRate = 2.f;
	bool bCanGrow = true;
};


UCLASS()
class RPGPROJECT_API URPGPoolManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void CreateNewPool(const FString& PoolKey, TSubclassOf<UObject> PoolObjectClass, const int32 InitPoolSize, const int32 MaxPoolSize, const bool bCanGrow, float GrowRate = 1.5f);
	UObject* GetPoolObject(const FString& PoolKey);
	
	void ReturnToPool(const FString& PoolKey, UObject* ReturnObject);
	void ClearAll();
	
private:
	UObject* CreateNewPoolObject(const FString& PoolKey, TSubclassOf<UObject>& PoolObjectClass) const;

	void OnWorldDestroyed(UWorld* World);

private:
	UPROPERTY()
	TMap<FString, FRPGObjectPool> ObjectPools;
	
	
};
