

#pragma once

#include "CoreMinimal.h"
#include "UI/Tooltip/RPGTooltipWidget.h"
#include "RPGItemTooltipWidget.generated.h"


struct FRPGItemDefinition;
class URPGItemOptionRowWidget;


UCLASS()
class RPGPROJECT_API URPGItemTooltipWidget : public URPGTooltipWidget
{
	GENERATED_BODY()

public:
	virtual void Init(const URPGTooltipContent* TooltipContent) override;
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Name;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Description;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_OptionTexts;
	
	
};
