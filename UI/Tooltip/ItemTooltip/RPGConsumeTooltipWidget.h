// 

#pragma once

#include "CoreMinimal.h"
#include "RPGItemTooltipWidget.h"
#include "UObject/Object.h"
#include "RPGConsumeTooltipWidget.generated.h"

class URPGTooltipContent;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGConsumeTooltipWidget : public URPGItemTooltipWidget
{
	GENERATED_BODY()

public:
	virtual void Init(const URPGTooltipContent* TooltipContent) override;
	
	
};
