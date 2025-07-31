#include "RPGMovableWidget.h"
#include "RPGHelperLibrary.h"
#include "Components/CanvasPanelSlot.h"

FReply URPGMovableWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bMove = true;

	RPGHelper::GetMousePosition(this,PrevMousePos);
	
	return FReply::Handled();
}

FReply URPGMovableWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bMove = false;
	
	return FReply::Handled();
}

void URPGMovableWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bMove = false;
}

void URPGMovableWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bMove)
	{
		FVector2D NewMousePos;
		RPGHelper::GetMousePosition(this,NewMousePos);

		FVector2D DeltaMousePos = NewMousePos - PrevMousePos;
		PrevMousePos = NewMousePos;

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + DeltaMousePos);
		}
	}
	

}
