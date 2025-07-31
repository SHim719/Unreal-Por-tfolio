// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/PoolInterface.h"
#include "RPGPoolWidget.generated.h"

UCLASS()
class RPGPROJECT_API URPGPoolWidget : public UUserWidget, public IPoolInterface
{
	GENERATED_BODY()

public:
	virtual void OnExitPool() override;
	virtual void OnReturnToPool() override;
	void SetPoolKey(const FString& InPoolKey) { PoolKey = InPoolKey;}

protected:
	virtual void ReturnToPool();

protected:
	UPROPERTY(EditDefaultsOnly)
	FString PoolKey;
};
