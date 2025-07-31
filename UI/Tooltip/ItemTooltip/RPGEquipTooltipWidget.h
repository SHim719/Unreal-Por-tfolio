// 

#pragma once

#include "CoreMinimal.h"
#include "RPGItemTooltipWidget.h"
#include "Data/Item/RPGItemOption.h"
#include "RPGEquipTooltipWidget.generated.h"


UCLASS()
class RPGPROJECT_API URPGEquipTooltipWidget : public URPGItemTooltipWidget
{
	GENERATED_BODY()

protected:
	virtual void Init(const URPGTooltipContent* TooltipContent) override;

private:
	void AddItemOptionRowText(const TArray<FRPGItemOptionInstance>& ItemOptionInstances, TArray<FText>& OptionTexts);


private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_RandomOptionTexts;
	
};
