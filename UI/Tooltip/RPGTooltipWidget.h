

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGTooltipWidget.generated.h"


class URPGTooltipContent;
class UTextBlock;
class UImage;
class UVerticalBox;

UCLASS()
class RPGPROJECT_API URPGTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void Init(const URPGTooltipContent* TooltipContent) PURE_VIRTUAL(virtual void Init(const URPGTooltipContent* TooltipContent));
	
};
