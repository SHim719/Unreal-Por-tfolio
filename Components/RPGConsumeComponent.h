

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGConsumeComponent.generated.h"


class URPGItemInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API URPGConsumeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URPGConsumeComponent();

	bool Consume(const URPGItemInstance* ItemInstance, const int32 InventorySlotIndex) const;


};
