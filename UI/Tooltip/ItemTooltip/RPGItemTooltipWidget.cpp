#include "RPGItemTooltipWidget.h"

#include "RPGHelperLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Item/RPGItemData.h"
#include "UI/Tooltip/TooltipContent/RPGTooltipContent.h"

void URPGItemTooltipWidget::Init(const URPGTooltipContent* TooltipContent)
{
	if (TooltipContent == nullptr)
		return;

	const URPGItemTooltipContent* ItemTooltipContent = Cast<URPGItemTooltipContent>(TooltipContent);
	if (ItemTooltipContent == nullptr)
		return;

	const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(this, ItemTooltipContent->ItemId);
	if (ItemDef == nullptr)
		return;

	if (Image_Icon)
		Image_Icon->SetBrushFromTexture(ItemDef->IconTexture);

	if (Text_Name)
		Text_Name->SetText(ItemDef->DisplayName);

	if (Text_Description)
		Text_Description->SetText(ItemDef->ItemDescription);
	
}
