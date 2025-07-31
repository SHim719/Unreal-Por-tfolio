#include "RPGSlotWidget.h"

#include "RPGDragDropOperation.h"
#include "RPGDragWidget.h"
#include "RPGHelperLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Message/RPGGameMessage.h"
#include "UI/Tooltip/RPGTooltipManager.h"



void URPGSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Image_Icon->SetVisibility(ESlateVisibility::Collapsed);
	Text_Count->SetVisibility(ESlateVisibility::Collapsed);
	Image_Cooldown->SetVisibility(ESlateVisibility::Collapsed);
	Text_Cooldown->SetVisibility(ESlateVisibility::Collapsed);

	UpdateSlotActiveState(true);
}

void URPGSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	bMouseOnSlot = true;
	if (TooltipContent)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnMouseEnter"));
		
		ShowTooltip();
	}
}

void URPGSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bMouseOnSlot = false;
	if (TooltipContent)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnMouseLeave"));
	
		HideTooltip();
	}

}


FReply URPGSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();

	const FKey Key = InMouseEvent.GetEffectingButton();

	if (Key == EKeys::LeftMouseButton)
	{
		if (IsEmpty() == false && bCanDragDrop == true)
		{
			Reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
		}
	}
	else if (Key == EKeys::RightMouseButton)
	{
		BroadcastSlotInteraction();
	}

	
	return Reply;
}

void URPGSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	if (DragSlotClass == nullptr)
		return;

	if (URPGDragWidget* DragSlot = CreateWidget<URPGDragWidget>(this, DragSlotClass))
	{
		UTexture2D* IconTex = Cast<UTexture2D>(Image_Icon->GetBrush().GetResourceObject());
		if (IconTex == nullptr)
			return;

		DragSlot->SetIcon(IconTex);

		URPGDragDropOperation* DragDropOperation = NewObject<URPGDragDropOperation>();
		if (DragDropOperation)
		{
			DragDropOperation->DefaultDragVisual = DragSlot;
			DragDropOperation->FromSlotType = SlotType;
			DragDropOperation->SlotIndex = SlotIndex;
		}
		OutOperation = DragDropOperation;
	}
}

bool URPGSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	URPGDragDropOperation* Operation = Cast<URPGDragDropOperation>(InOperation);
	if (Operation == nullptr)
		return false;

	FDragDropMsg DragDropMsg;
	DragDropMsg.FromSlotType = Operation->FromSlotType;
	DragDropMsg.FromSlotIndex = Operation->SlotIndex;
	DragDropMsg.ToSlotType = SlotType;
	DragDropMsg.ToSlotIndex = SlotIndex;
	RPGMessage::BroadcastMessage(GetWorld(), DragDropMsg, FGameplayTag::RequestGameplayTag(FName("Widget.Slot.DragDrop")));
	
	return true;
}

void URPGSlotWidget::UpdateSlot(UTexture2D* Thumbnail, const int32 ItemCount) const
{
	if (Thumbnail)
	{
		Image_Icon->SetVisibility(ESlateVisibility::Visible);
		Image_Icon->SetBrushFromTexture(Thumbnail);
	}

	if (ItemCount >= 1)
	{
		if (ItemCount > 1)
		{
			Text_Count->SetText(FText::AsNumber(ItemCount));
			Text_Count->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Text_Count->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		Image_Icon->SetBrushFromTexture(nullptr);
		Image_Icon->SetVisibility(ESlateVisibility::Collapsed);
		Text_Count->SetVisibility(ESlateVisibility::Collapsed);

		Text_Cooldown->SetVisibility(ESlateVisibility::Collapsed);
		Image_Cooldown->SetVisibility(ESlateVisibility::Collapsed);
	}
	

}

void URPGSlotWidget::UpdateTooltipContent(const URPGTooltipContent* InTooltipContent)
{
	if (bMouseOnSlot)
	{
		if (InTooltipContent == nullptr)
		{
			HideTooltip();
		}
		else if (TooltipContent != InTooltipContent)
		{
			TooltipContent = InTooltipContent;
			ShowTooltip();
		}
	}

	if (TooltipContent != InTooltipContent)
		TooltipContent = InTooltipContent;
}

void URPGSlotWidget::UpdateCooldown(float RemainingTime, float CooldownTime)
{
	if (RemainingTime > 0.f)
	{
		Text_Cooldown->SetVisibility(ESlateVisibility::Visible);
		Image_Cooldown->SetVisibility(ESlateVisibility::Visible);
		
		float ProgressRatio = RemainingTime / CooldownTime;
		if (auto* CooldownMaterial = UWidgetBlueprintLibrary::GetDynamicMaterial(const_cast<FSlateBrush&>(Image_Cooldown->GetBrush())))
		{
			CooldownMaterial->SetScalarParameterValue(FName("ProgressPercent"), ProgressRatio);
		}
		UpdateCooldownText(RemainingTime);
	}
	else
	{
		Text_Cooldown->SetVisibility(ESlateVisibility::Collapsed);
		Image_Cooldown->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URPGSlotWidget::UpdateCooldownText(float RemainingTime) const
{
	FNumberFormattingOptions FormattingOptions;
	if (RemainingTime >= 1.f)
	{
		FormattingOptions.MaximumFractionalDigits = 0; 
	}
	else
	{
		FormattingOptions.MaximumFractionalDigits = 1; 
	}

	
	Text_Cooldown->SetText(FText::AsNumber(RemainingTime, &FormattingOptions));
}


bool URPGSlotWidget::IsEmpty() const
{
	return Image_Icon->GetBrush().GetResourceObject() == nullptr;
}

void URPGSlotWidget::UpdateSlotActiveState(const bool bActive)
{
	bSlotActive = bActive;
	if (bActive)
	{
		const FLinearColor ActiveColor = FLinearColor::White;
		Image_Icon->SetColorAndOpacity(ActiveColor);
	}
	else
	{
		constexpr FLinearColor InactiveColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);
		Image_Icon->SetColorAndOpacity(InactiveColor);
	}
}

void URPGSlotWidget::BroadcastSlotInteraction() const
{
	const FSlotInteractionMsg SlotInteractionMsg(SlotType, SlotIndex);
	RPGMessage::BroadcastMessage(GetWorld(), SlotInteractionMsg, FGameplayTag::RequestGameplayTag(FName("Widget.Slot.Interaction")));
	
}


void URPGSlotWidget::ShowTooltip()
{
	if (auto TooltipManager = RPGHelper::GetTooltipManager(this))
	{
		FVector2d PixelPosition, ViewportPosition;
		USlateBlueprintLibrary::LocalToViewport(this, GetCachedGeometry(), FVector2D::ZeroVector, PixelPosition, ViewportPosition);
		
		TooltipManager->ShowTooltip(TooltipContent, PixelPosition, GetCachedGeometry().GetAbsoluteSize());
	}
}

void URPGSlotWidget::HideTooltip()
{
	if (auto TooltipManager = RPGHelper::GetTooltipManager(this))
	{
		TooltipManager->HideTooltip(TooltipContent);
	}
}






