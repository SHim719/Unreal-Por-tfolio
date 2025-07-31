#include "RPGGridSlots.h"

#include "RPGSlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"


void URPGGridSlots::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TotalSlots > 0 && SlotsPerRow > 0)
	{
		UpdateSlotCount(TotalSlots);
	}
	
}

void URPGGridSlots::HideAllSlots()
{
	for (const auto SlotWidget : SlotWidgets)
	{
		SlotWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void URPGGridSlots::UpdateSlotCount(const int32 SlotCount)
{
	TotalSlots = SlotCount;
	
	SlotWidgets.Reserve(TotalSlots);
	GridSlots.Reserve(TotalSlots);

	AddSlots();
}

void URPGGridSlots::SetSlotType(const ESlotType Type)
{
	for (const auto SlotWidget : SlotWidgets)
	{
		SlotWidget->SetSlotType(Type);
	}
}

void URPGGridSlots::ClearAllSlots()
{
	for (auto SlotWidget : SlotWidgets)
	{
		SlotWidget->UpdateSlot(nullptr, 0);
	}
}


void URPGGridSlots::AddSlots()
{
	for (int32 i = 1; i <= TotalSlots; ++i)
	{
		if (auto slot = CreateWidget<URPGSlotWidget>(this, SlotWidgetClass))
		{
			const int32 Index = SlotWidgets.Add(slot);
			slot->SetSlotIndex(Index);
			slot->SetCanDragDrop(bCanDragDrop);
			AddSlotToGrid(Index, slot);
		}
	}
}

void URPGGridSlots::AddSlotToGrid(const int32 Index, URPGSlotWidget* ItemSlot)
{
	if (Grid)
	{
		const auto GridSlot = Grid->AddChildToUniformGrid(ItemSlot);
		SetSlotRowCol(GridSlot, Index);
		GridSlots.Add(GridSlot);
	}
}

void URPGGridSlots::SetSlotRowCol(UUniformGridSlot* GridSlot, const int32 Index) const
{
	const int32 Row = Index / SlotsPerRow;
	const int32 Col = Index % SlotsPerRow;

	GridSlot->SetRow(Row);
	GridSlot->SetColumn(Col);
}
