#include "UI/RPGHUD.h"
#include "RPGHelperLibrary.h"
#include "ViewModel/MVVM_ProgressBar.h"
#include "RPGMainWidget.h"
#include "ObjectPool/RPGPoolManager.h"
#include "DamageText/RPGDamageText.h"
#include "Tooltip/RPGTooltipManager.h"




void ARPGHUD::Initialize()
{
	VM_GlobeHpBar = NewObject<UMVVM_ProgressBar>(this, UMVVM_ProgressBar::StaticClass());
	VM_ExpBar = NewObject<UMVVM_ProgressBar>(this, UMVVM_ProgressBar::StaticClass());
	
	MainWidget = CreateWidget<URPGMainWidget>(GetGameInstance(), MainWidgetClass);
	if (MainWidget)
	{
		MainWidget->AddToViewport();
	}

	if (auto* TooltipManager = RPGHelper::GetTooltipManager(this))
	{
		TooltipManager->InitializeTooltips();
	}
	
	if (DamageTextWidgetClass)
	{
		RPGHelper::GetPoolManager(this)->CreateNewPool("DamageNumber", DamageTextWidgetClass, 20, 50, true);
	}
}

TSubclassOf<URPGTooltipWidget> ARPGHUD::GetTooltipWidgetClass(const ERPGTooltipType TooltipType) const
{
	auto Val = TooltipWidgetClasses.Find(TooltipType);
	if (Val == nullptr)
		return nullptr;
	
	return *Val;
}


