// 

#pragma once

#include "CoreMinimal.h"
#include "RPGNpcBase.h"
#include "RPGShopNpc.generated.h"

UCLASS()
class RPGPROJECT_API ARPGShopNpc : public ARPGNpcBase
{
	GENERATED_BODY()

public:
	ARPGShopNpc();
	virtual void BeginPlay() override;
	virtual void Interact() override;
	
};
