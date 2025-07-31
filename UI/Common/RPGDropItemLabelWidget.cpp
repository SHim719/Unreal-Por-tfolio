#include "RPGDropItemLabelWidget.h"
#include "Components/TextBlock.h"

FReply URPGDropItemLabelWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnClickItemLabel.ExecuteIfBound();
	return FReply::Handled();
}

void URPGDropItemLabelWidget::SetLabelText(const FText& InText, const int32 ItemCount)
{
	if (Text_ItemLabel)
	{
		if (ItemCount > 1)
		{
			const FString FinalString = InText.ToString() + FString::Printf(TEXT(" %d"), ItemCount);
			Text_ItemLabel->SetText(FText::FromString(FinalString));
		}
		else
		{
			Text_ItemLabel->SetText(InText);
		}
		
	}
}
