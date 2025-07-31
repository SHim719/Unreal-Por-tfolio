

#pragma once

#include "CoreMinimal.h"
#include "RPGTargetCircle.generated.h"


DECLARE_DELEGATE_TwoParams(FOnConfirmedPosition, bool bCanceled, const FVector& ConfirmedPosition)


class ARPGPlayerController;

UCLASS()
class RPGPROJECT_API ARPGTargetCircle : public AActor
{
	GENERATED_BODY()

public:
	ARPGTargetCircle();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void ShowTargetCircle(const float Radius);
	void HideTargetCircle();
	void Confirm();

	bool IsActive() const { return bIsActive;}

public:
	FOnConfirmedPosition OnConfirmedPosition;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDecalComponent> DecalComponent;
	
	UPROPERTY()
	TWeakObjectPtr<ARPGPlayerController> PC;
	
	bool bIsActive = false;
};
