#include "RPGTooltipManager.h"

#include "RPGHelperLibrary.h"
#include "RPGTooltipWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Slate/SGameLayerManager.h"
#include "UI/RPGHUD.h"


void URPGTooltipManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Tooltips.SetNum(static_cast<int32>(ERPGTooltipType::End));
}

void URPGTooltipManager::InitializeTooltips()
{
	if (auto* HUD = RPGHelper::GetHUD_Safe(this))
	{
		for (int32 i = 0; i < static_cast<int32>(ERPGTooltipType::End); ++i)
		{
			if (TSubclassOf<URPGTooltipWidget> TooltipWidgetClass = HUD->GetTooltipWidgetClass(static_cast<ERPGTooltipType>(i)))
			{
				Tooltips[i] = CreateWidget<URPGTooltipWidget>(GetGameInstance(), TooltipWidgetClass);
				Tooltips[i]->AddToViewport();
			}
		}
	}
	
}

void URPGTooltipManager::ShowTooltip(const URPGTooltipContent* TooltipContent, const FVector2D& RefWidgetPos, const FVector2D& RefWidgetSize)
{
	if (TooltipContent == nullptr || CurrentTooltipContent == TooltipContent)
		return;
	
	URPGTooltipWidget* Tooltip = Tooltips[static_cast<int32>(TooltipContent->TooltipType)];
	if (Tooltip == nullptr)
		return;

	CurrentTooltipContent = TooltipContent;

	Tooltip->Init(TooltipContent);
	Tooltip->SetVisibility(ESlateVisibility::HitTestInvisible);
	Tooltip->SetRenderOpacity(0.f);
	Tooltip->SetPositionInViewport(GetDesiredTooltipPosition(RefWidgetPos, RefWidgetSize));

	GetGameInstance()->GetTimerManager().ClearTimer(ShowTooltipTimer);
	GetGameInstance()->GetTimerManager().SetTimer(ShowTooltipTimer,
	FTimerDelegate::CreateLambda([this, RefWidgetPos, RefWidgetSize]()
	{
		if (CurrentTooltipContent == nullptr)
			return;
			
		if (URPGTooltipWidget* Tooltip = Tooltips[static_cast<int32>(CurrentTooltipContent->TooltipType)])
		{
			Tooltip->SetPositionInViewport(GetDesiredTooltipPosition(RefWidgetPos, RefWidgetSize));
			Tooltip->SetRenderOpacity(1.f);
		}
	}), 0.1f, false);
	
}

void URPGTooltipManager::HideTooltip(const URPGTooltipContent* TooltipContent)
{
	if (TooltipContent == nullptr || TooltipContent->TooltipType >= ERPGTooltipType::End || TooltipContent != CurrentTooltipContent)
		return;

	CurrentTooltipContent = nullptr;
	if (URPGTooltipWidget* Tooltip = Tooltips[static_cast<int32>(TooltipContent->TooltipType)])
	{
		Tooltip->SetVisibility(ESlateVisibility::Collapsed);
		GetGameInstance()->GetTimerManager().ClearTimer(ShowTooltipTimer);
		ShowTooltipTimer.Invalidate();
	}
}


FVector2D URPGTooltipManager::GetCurrentTooltipSize() const
{
	if (CurrentTooltipContent == nullptr)
		return FVector2D::Zero();

	URPGTooltipWidget* Tooltip = Tooltips[static_cast<int32>(CurrentTooltipContent->TooltipType)];
	if (Tooltip == nullptr)
		return FVector2D::Zero();

	auto Geometry = Tooltip->GetCachedGeometry();
	return Geometry.GetDrawSize();
}


FVector2D URPGTooltipManager::GetDesiredTooltipPosition(const FVector2D& RefWidgetPos, const FVector2D& RefWidgetSize) const
{
	auto* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC == nullptr)
		return FVector2D::ZeroVector;
	
	FVector2D TooltipPosition = FVector2D::ZeroVector;
	FVector2D CurTooltipSize = GetCurrentTooltipSize();

	FIntVector ViewportSize;
	PC->GetViewportSize(ViewportSize.X, ViewportSize.Y);
	
	TooltipPosition = RefWidgetPos;
	TooltipPosition.X +=RefWidgetSize.X + 5.f;
	
	if (TooltipPosition.X + CurTooltipSize.X > ViewportSize.X)
	{
		TooltipPosition.X = RefWidgetPos.X - CurTooltipSize.X - 5.f;
	}

	if (TooltipPosition.Y + CurTooltipSize.Y > ViewportSize.Y)
	{
		TooltipPosition.Y = ViewportSize.Y - CurTooltipSize.Y;
	}
	
	return TooltipPosition;
}
