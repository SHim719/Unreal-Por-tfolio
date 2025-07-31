// 

#pragma once

#include "CoreMinimal.h"
#include "TooltipContent/RPGTooltipContent.h"
#include "RPGTooltipManager.generated.h"


class URPGTooltipWidget;

UCLASS()
class RPGPROJECT_API URPGTooltipManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void InitializeTooltips();
	
	void ShowTooltip(const URPGTooltipContent* TooltipContent, const FVector2D& RefWidgetPos, const FVector2D& RefWidgetSize);
	void HideTooltip(const URPGTooltipContent* TooltipContent);
	
	FVector2D GetCurrentTooltipSize() const;
private:
	FVector2D GetDesiredTooltipPosition(const FVector2D& RefWidgetPos, const FVector2D& RefWidgetSize) const;
	
private:
	UPROPERTY()
	TArray<TObjectPtr<URPGTooltipWidget>> Tooltips;

	UPROPERTY()
	TObjectPtr<const URPGTooltipContent> CurrentTooltipContent;

	FTimerHandle ShowTooltipTimer;
}; 
