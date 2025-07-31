// 

#pragma once

#include "CoreMinimal.h"
#include "Actor/RPGPoolActor.h"
#include "GameFramework/Actor.h"
#include "RPGDropItemActor.generated.h"

class URPGDropItemLabelWidget;
class UWidgetComponent;

UCLASS()
class RPGPROJECT_API ARPGDropItemActor : public ARPGPoolActor
{
	GENERATED_BODY()

public:
	ARPGDropItemActor();

	void Init(const FName& InItemId, const int32 InItemCount);

	virtual void OnExitPool() override;
	virtual void OnReturnToPool() override;
	


private:
	void RequestInsertItem();

	
private:
	FName ItemId;

	int32 ItemCount = 0;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> ItemMeshComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> ItemLabelWidgetComponent;
};
