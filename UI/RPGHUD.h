

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Tooltip/TooltipContent/RPGTooltipContent.h"
#include "RPGHUD.generated.h"


class URPGConfirmWidget;
class URPGDropItemLabelWidget;
class URPGDecideCountWidget;
class URPGTooltipWidget;
class URPGMainWidget;
class URPGDamageText;
class UMVVM_ProgressBar;

UCLASS()
class RPGPROJECT_API ARPGHUD : public AHUD
{
	GENERATED_BODY()

public:
	void Initialize();
	URPGMainWidget* GetMainWidget() const {return MainWidget;}
	TSubclassOf<URPGTooltipWidget> GetTooltipWidgetClass(const ERPGTooltipType TooltipType) const;
	TSubclassOf<URPGDecideCountWidget> GetSplitQuantityWidgetClass() const { return SplitQuantityWidgetClass;}
	TSubclassOf<URPGConfirmWidget> GetConfirmWidgetClass() const { return ConfirmWidgetClass;}

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_ProgressBar> VM_GlobeHpBar;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_ProgressBar> VM_ExpBar;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPGMainWidget> MainWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TMap<ERPGTooltipType, TSubclassOf<URPGTooltipWidget>> TooltipWidgetClasses;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPGDecideCountWidget> SplitQuantityWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPGDamageText> DamageTextWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPGConfirmWidget> ConfirmWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<URPGMainWidget> MainWidget;
	
};
