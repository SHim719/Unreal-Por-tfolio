#include "RPGEquipSampleTooltipWidget.h"
#include "RPGHelperLibrary.h"
#include "Components/TextBlock.h"
#include "Data/Item/RPGItemData.h"
#include "Item/Fragment/RPGItemFragment_Equippable.h"
#include "UI/Tooltip/TooltipContent/RPGTooltipContent.h"

void URPGEquipSampleTooltipWidget::Init(const URPGTooltipContent* TooltipContent)
{
	Super::Init(TooltipContent);

	
	const URPGItemTooltipContent* ItemTooltipContent = Cast<URPGItemTooltipContent>(TooltipContent);
	if (ItemTooltipContent == nullptr)
		return;

	const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(this, ItemTooltipContent->ItemId);
	if (ItemDef == nullptr)
		return;

	TArray<FText> TooltipTexts;
	TooltipTexts.Add(FText::FromString(TEXT("기본 옵션")));
	

	const URPGItemFragment_Equippable* ItemFragment_Equippable = ItemDef->FindFragmentByClass<URPGItemFragment_Equippable>();
	if (ItemFragment_Equippable == nullptr)
		return;
	
	for (auto& ItemOptionInfo : ItemFragment_Equippable->MainOptionInfos)
	{
		if (const FRPGItemOptionRow* OptionData = ItemOptionInfo.OptionDataRowHandle.GetRow<FRPGItemOptionRow>(TEXT("Quert ItemOption")))
		{
			FFormatNamedArguments Arguments;

			for (auto& ItemOptionRange : ItemOptionInfo.OptionValueRanges)
			{
				FString KeyString = ItemOptionRange.ValueTag.GetTagName().ToString();
				FText ValueText = FText::FromString(FString::Printf(TEXT("(%d ~ %d)"), ItemOptionRange.MinValue, ItemOptionRange.MaxValue));
				Arguments.Add(KeyString, ValueText);
			}

			TooltipTexts.Add(FText::Format(OptionData->OptionText, Arguments));
		}
		
	}
	
	TooltipTexts.Add(FText::FromString(FString::Printf(TEXT("랜덤으로 %d개의 옵션이 부여됩니다."), ItemFragment_Equippable->RandomOptionCount)));
	
	if (Text_OptionTexts)
	{
		Text_OptionTexts->SetText( FText::Join(FText::FromString(TEXT("\n")), TooltipTexts));
	}
}
