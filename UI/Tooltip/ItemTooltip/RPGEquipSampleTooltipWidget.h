// 

#pragma once

#include "CoreMinimal.h"
#include "RPGItemTooltipWidget.h"
#include "RPGEquipSampleTooltipWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGEquipSampleTooltipWidget : public URPGItemTooltipWidget
{
	GENERATED_BODY()

protected:
	virtual void Init(const URPGTooltipContent* TooltipContent) override;
};
