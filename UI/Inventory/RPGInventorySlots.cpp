#include "UI/Inventory/RPGInventorySlots.h"
#include "RPGDefines.h"
#include "RPGProject.h"
#include "Item/RPGItemInstance.h"
#include "UI/Slot/RPGGridSlots.h"
#include "Message/RPGGameMessage.h"


void URPGInventorySlots::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BindWidgets();
	RegisterEvents();
}

void URPGInventorySlots::NativeConstruct()
{
	Super::NativeConstruct();
	
	InventorySlots[static_cast<int32>(EItemType::Equipment)]->SetVisibility(ESlateVisibility::Visible);
}

void URPGInventorySlots::BindWidgets()
{
	InventorySlots.SetNum(static_cast<int32>(EItemType::Etc) + 1);
	
	InventorySlots[static_cast<int32>(EItemType::Equipment)] = Cast<URPGGridSlots>(GetWidgetFromName("EquipmentSlots"));
	check(InventorySlots[static_cast<int32>(EItemType::Equipment)]);

	InventorySlots[static_cast<int32>(EItemType::Consume)] = Cast<URPGGridSlots>(GetWidgetFromName("ConsumeSlots"));
	check(InventorySlots[static_cast<int32>(EItemType::Consume)]);
	
	InventorySlots[static_cast<int32>(EItemType::Etc)] = Cast<URPGGridSlots>(GetWidgetFromName("EtcSlots"));
	check(InventorySlots[static_cast<int32>(EItemType::Etc)]);
}

void URPGInventorySlots::RegisterEvents()
{
	RPGMessage::RegisterListener(GetWorld(), FGameplayTag::RequestGameplayTag(TEXT("Widget.Inventory.UpdateSlotSize")), this, &ThisClass::HandleUpdateInventorySize);
	RPGMessage::RegisterListener(GetWorld(), FGameplayTag::RequestGameplayTag(TEXT("Widget.Inventory.UpdateSlot")), this, &ThisClass::HandleUpdateInventorySlot);
}

void URPGInventorySlots::HandleUpdateInventorySize(FGameplayTag Tag, const FInitInventorySizeMsg& Msg)
{
	int32 Index = 0;
	for (const auto& SlotsGrid: InventorySlots)
	{
		if (SlotsGrid)
		{
			SlotsGrid->UpdateSlotCount(Msg.InventorySize);
			SlotsGrid->SetSlotType(static_cast<ESlotType>(Index));
		}
		else
		{
			UE_LOG(LogRPGProject, Warning, TEXT("SlotsGrid is null"));
		}
		++Index;
	}
}

void URPGInventorySlots::HandleUpdateInventorySlot(FGameplayTag Tag, const FUpdateInventorySlotMsg& Msg)
{
	if (const auto SlotWidget = InventorySlots[static_cast<int32>(Msg.ItemType)]->GetSlotWidget(Msg.SlotIndex))
	{
		SlotWidget->UpdateSlot(Msg.Thumbnail, Msg.ItemCount);
		SlotWidget->UpdateTooltipContent(Msg.TooltipContent);
	}
	else
	{
		UE_LOG(LogRPGProject, Warning, TEXT("SlotWidget is null"));
	}
}



void URPGInventorySlots::ShowSlots(const int32 ItemTypeToInt32)
{
	for (const auto& Slots : InventorySlots)
	{
		Slots->SetVisibility(ESlateVisibility::Hidden);
	}
	
	InventorySlots[ItemTypeToInt32]->SetVisibility(ESlateVisibility::Visible);
}
