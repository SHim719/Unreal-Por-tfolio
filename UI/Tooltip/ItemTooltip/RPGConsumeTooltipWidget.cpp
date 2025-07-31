// 


#include "RPGConsumeTooltipWidget.h"

#include "RPGHelperLibrary.h"
#include "Components/TextBlock.h"
#include "Data/Item/RPGItemData.h"
#include "Data/Item/RPGItemOption.h"
#include "Item/ConsumeAttribute/RPGConsumeAttribute.h"
#include "Item/Fragment/RPGItemFragment_Consume.h"
#include "UI/Tooltip/TooltipContent/RPGTooltipContent.h"

void URPGConsumeTooltipWidget::Init(const URPGTooltipContent* TooltipContent)
{
	Super::Init(TooltipContent);
	
	const URPGItemTooltipContent* ItemTooltipContent = Cast<URPGItemTooltipContent>(TooltipContent);
	if (ItemTooltipContent == nullptr)
		return;

	const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(this, ItemTooltipContent->ItemId);
	if (ItemDef == nullptr)
		return;

	TArray<FText> TooltipTexts;
	TooltipTexts.Add(FText::FromString(TEXT("사용시")));
	
	if (const URPGItemFragment_Consume* ItemFragment_Consume = ItemDef->FindFragmentByClass<URPGItemFragment_Consume>())
	{
		for (auto ConsumeAttribute : ItemFragment_Consume->ConsumeAttributes)
		{
			if (ConsumeAttribute)
			{
				TooltipTexts.Add(ConsumeAttribute->GetTooltipText(this));
			}
		}
	}

	if (Text_OptionTexts)
	{
		Text_OptionTexts->SetText( FText::Join(FText::FromString(TEXT("\n")), TooltipTexts));
	}
	
}
