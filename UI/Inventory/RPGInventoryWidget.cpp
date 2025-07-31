#include "UI/Inventory/RPGInventoryWidget.h"
#include "RPGDefines.h"
#include "RPGInventorySlots.h"
#include "Components/TextBlock.h"
#include "Message/RPGGameMessage.h"
#include "UI/Common/RPGIndexedButton.h"


void URPGInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	BindWidgets();

	RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Inventory.UpdateGold")),
		this, &ThisClass::HandleChangedGoldMsg);
}


void URPGInventoryWidget::BindWidgets()
{
	InventoryTabButtons.SetNum(static_cast<int32>(EItemType::Etc) + 1);
	
	InventoryTabButtons[static_cast<int32>(EItemType::Equipment)] = Cast<URPGIndexedButton>(GetWidgetFromName("EquipmentButton"));
	check(InventoryTabButtons[static_cast<int32>(EItemType::Equipment)]);
	
	InventoryTabButtons[static_cast<int32>(EItemType::Consume)] = Cast<URPGIndexedButton>(GetWidgetFromName("ConsumeButton"));
	check(InventoryTabButtons[static_cast<int32>(EItemType::Consume)]);

	InventoryTabButtons[static_cast<int32>(EItemType::Etc)] = Cast<URPGIndexedButton>(GetWidgetFromName("EtcButton"));
	check(InventoryTabButtons[static_cast<int32>(EItemType::Etc)]);

	for (int32 i = 0; i <= static_cast<int32>(EItemType::Etc); ++i)
	{
		InventoryTabButtons[i]->OnButtonClicked.BindUObject(this, &ThisClass::HandleInventoryTabClicked);
		InventoryTabButtons[i]->SetIndex(i);
	}
}


void URPGInventoryWidget::HandleInventoryTabClicked(int32 ButtonIndex)
{
	if (InventorySlots)
		InventorySlots->ShowSlots(ButtonIndex);
}

void URPGInventoryWidget::HandleChangedGoldMsg(FGameplayTag, const FGoldChangedMsg& Msg)
{
	if (GoldText)
	{
		GoldText->SetText(FText::AsNumber(Msg.Gold));
	}
}





