

#pragma once

#include "CoreMinimal.h"
#include "Item/Fragment/RPGItemFragment.h"
#include "RPGDefines.h"
#include "RPGItemFragment_Consume.generated.h"


class URPGConsumeAttribute;

UCLASS()
class RPGPROJECT_API URPGItemFragment_Consume : public URPGItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<URPGConsumeAttribute>> ConsumeAttributes;
};
