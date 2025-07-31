// 

#pragma once

#include "CoreMinimal.h"
#include "RPGConsumeAttribute.h"
#include "RPGConsumeAttribute_GiveItems.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGConsumeAttribute_GiveItems : public URPGConsumeAttribute
{
	GENERATED_BODY()

public:
	virtual bool Consume(AActor* Player) override;
	virtual FText GetTooltipText(const UObject* WorldContextObject) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, int32> GiveItems;
};
