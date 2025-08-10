// 

#pragma once

#include "CoreMinimal.h"
#include "RPGEnemy.h"
#include "RPGNormalEnemy.generated.h"

UCLASS()
class RPGPROJECT_API ARPGNormalEnemy : public ARPGEnemy
{
	GENERATED_BODY()

public:
	ARPGNormalEnemy();

protected:
	virtual void BeginPlay() override;
	virtual UUserWidget* GetHpBarWidget() const override;


protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> HpBarComponent;
	
};
