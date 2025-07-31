#include "UI/RPGMainWidget.h"
#include "RPGDefines.h"
#include "Inventory/RPGInventoryWidget.h"

void URPGMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Widgets.SetNum(static_cast<int32>(ERPGWidgetType::End));
	Widgets[static_cast<int32>(ERPGWidgetType::Inventory)] = Cast<UUserWidget>(GetWidgetFromName(TEXT("InventoryWidget"))); 
	Widgets[static_cast<int32>(ERPGWidgetType::SkillTree)] = Cast<UUserWidget>(GetWidgetFromName(TEXT("SkillTreeWidget"))); 
	Widgets[static_cast<int32>(ERPGWidgetType::Quest)] = Cast<UUserWidget>(GetWidgetFromName(TEXT("QuestListWidget"))); 
	
}


void URPGMainWidget::ToggleWidgetVisibility(const int32 WidgetIndex)
{
	if (Widgets[WidgetIndex] == nullptr)
		return;
	
	const ESlateVisibility CurVisibility = Widgets[WidgetIndex]->GetVisibility();
	if (CurVisibility == ESlateVisibility::Visible)
	{
		Widgets[WidgetIndex]->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Widgets[WidgetIndex]->SetVisibility(ESlateVisibility::Visible);
	}
	
}

