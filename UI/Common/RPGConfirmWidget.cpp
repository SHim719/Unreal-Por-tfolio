// 


#include "RPGConfirmWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void URPGConfirmWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Confirm)
	{
		Button_Confirm->OnClicked.AddDynamic(this, &ThisClass::OnClicked_ConfirmButton);
	}

	if (Button_Cancel)
	{
		Button_Cancel->OnClicked.AddDynamic(this, &ThisClass::OnClicked_CancelButton);
	}
}

void URPGConfirmWidget::SetDescriptionText(const FText& Text)
{
	if (Text_Description)
	{
		Text_Description->SetText(Text);
	}
}

void URPGConfirmWidget::OnClicked_ConfirmButton()
{
	OnConfirmButtonClicked.ExecuteIfBound();
	SetVisibility(ESlateVisibility::Collapsed);
}

void URPGConfirmWidget::OnClicked_CancelButton()
{
	OnCancelButtonClicked.ExecuteIfBound();
	SetVisibility(ESlateVisibility::Collapsed);
}
