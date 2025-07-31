// 


#include "RPGShopEntryWidget.h"

#include "RPGHelperLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Item/RPGItemData.h"
#include "UI/Slot/RPGSlotWidget.h"

void URPGShopEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Focus)
	{
		Button_Focus->OnClicked.AddDynamic(this, &ThisClass::FocusThisEntry);
	}
}


void URPGShopEntryWidget::Init(const FName& ItemId, const int32 EntryIndex)
{
	const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(this, ItemId);
	if (ItemDef == nullptr)
		return;
	
	if (ItemEntry)
	{
		ItemEntry->UpdateSlot(ItemDef->IconTexture, 1);
		ItemEntry->UpdateTooltipContent(RPGHelper::GetItemTooltipContent(this, ItemId));
	}

	if (Text_ItemName)
	{
		Text_ItemName->SetText(ItemDef->DisplayName);
	}

	if (Text_Price)
	{
		Text_Price->SetText(FText::AsNumber(ItemDef->BuyPrice));
	}

	CurrentEntryIndex = EntryIndex;
	RefItemId = ItemId;
}

void URPGShopEntryWidget::ReleaseFocus()
{
	if (Image_Background)
	{
		Image_Background->SetColorAndOpacity(OriginColor);
	}
	
}

void URPGShopEntryWidget::FocusThisEntry()
{
	OnFocusEntryDelegate.ExecuteIfBound(CurrentEntryIndex);

	if (Image_Background)
	{
		Image_Background->SetColorAndOpacity(FocusColor);
	}
}
