#include "UI/Equipment/RPGEquipmentWidget.h"
#include "RPGDefines.h"
#include "Item/RPGItemInstance.h"
#include "Message/RPGGameMessage.h"
#include "UI/Slot/RPGSlotWidget.h"


void URPGEquipmentWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BindWidget();
	RegisterEvents();
}

void URPGEquipmentWidget::BindWidget()
{
	EquipmentSlots.SetNum(static_cast<int32>(EEquipSlotType::End));
	
	EquipmentSlots[static_cast<int32>(EEquipSlotType::Helmets)] = Cast<URPGSlotWidget>(GetWidgetFromName("Slot_Helmet"));
	check(EquipmentSlots[static_cast<int32>(EEquipSlotType::Helmets)]);

	EquipmentSlots[static_cast<int32>(EEquipSlotType::Top)] = Cast<URPGSlotWidget>(GetWidgetFromName("Slot_Top"));
	check(EquipmentSlots[static_cast<int32>(EEquipSlotType::Top)]);

	EquipmentSlots[static_cast<int32>(EEquipSlotType::Bottom)] = Cast<URPGSlotWidget>(GetWidgetFromName("Slot_Bottom"));
	check(EquipmentSlots[static_cast<int32>(EEquipSlotType::Bottom)]);

	EquipmentSlots[static_cast<int32>(EEquipSlotType::Shoes)] = Cast<URPGSlotWidget>(GetWidgetFromName("Slot_Boots"));
	check(EquipmentSlots[static_cast<int32>(EEquipSlotType::Shoes)]);

	EquipmentSlots[static_cast<int32>(EEquipSlotType::Weapon)] = Cast<URPGSlotWidget>(GetWidgetFromName("Slot_Weapon"));
	check(EquipmentSlots[static_cast<int32>(EEquipSlotType::Weapon)]);


	for (int32 i = 0; i < static_cast<int32>(EEquipSlotType::End); ++i)
	{
		EquipmentSlots[i]->SetSlotIndex(i);
		EquipmentSlots[i]->SetSlotType(ESlotType::Equip);
	}
}

void URPGEquipmentWidget::RegisterEvents()
{
	RPGMessage::RegisterListener(GetWorld(), FGameplayTag::RequestGameplayTag("Widget.Equipment.UpdateSlot"), this, &ThisClass::HandleEquipmentSlotUpdated);
}


void URPGEquipmentWidget::HandleEquipmentSlotUpdated(FGameplayTag Tag, const FUpdateEquipmentSlotMsg& Msg)
{
	if (EquipmentSlots[Msg.SlotIndex])
	{
		EquipmentSlots[Msg.SlotIndex]->UpdateSlot(Msg.Thumbnail, Msg.Thumbnail == nullptr ? 0 : 1);
		EquipmentSlots[Msg.SlotIndex]->UpdateTooltipContent(Msg.TooltipContent);
	}
}
