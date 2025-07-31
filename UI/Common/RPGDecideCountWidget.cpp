#include "RPGDecideCountWidget.h"

#include "Components/Button.h"
#include "Components/SpinBox.h"

void URPGDecideCountWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Confirm)
	{
		Button_Confirm->OnClicked.AddDynamic(this, &ThisClass::OnConfirmButtonClicked);
	}

	if (Button_Cancel)
	{
		Button_Cancel->OnClicked.AddDynamic(this, &ThisClass::OnCancelButtonClicked);
	}
	
}

void URPGDecideCountWidget::Init(const int32 MaxValue)
{
	if (SpinBox)
	{
		SpinBox->SetMaxValue(MaxValue);
		SpinBox->SetMaxSliderValue(MaxValue);
		SpinBox->SetValue(0);
	}

	OnDecideCountDelegate.Unbind();
}

void URPGDecideCountWidget::OnConfirmButtonClicked()
{
	OnDecideCountDelegate.ExecuteIfBound(SpinBox->GetValue());
	SetVisibility(ESlateVisibility::Collapsed);
	OnDecideCountDelegate.Unbind();
}

void URPGDecideCountWidget::OnCancelButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
	OnDecideCountDelegate.Unbind();
	OnCancelDelegate.ExecuteIfBound();
}
