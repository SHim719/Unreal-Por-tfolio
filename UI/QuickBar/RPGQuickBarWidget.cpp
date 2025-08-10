
#include "UI/QuickBar/RPGQuickBarWidget.h"

#include "Components/TextBlock.h"
#include "Message/RPGGameMessage.h"
#include "UI/Slot/RPGSlotWidget.h"

void URPGQuickBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Slots.SetNum(QuickBarSlotNum);

	BindWidgets();
	RPGMessage::RegisterListener(GetWorld(), FGameplayTag::RequestGameplayTag(TEXT("Widget.QuickBar.UpdateSlot")), this, &ThisClass::HandleUpdateQuickBarSlot);
	RPGMessage::RegisterListener(GetWorld(), FGameplayTag::RequestGameplayTag(TEXT("Widget.QuickBar.UpdateCooldown")), this, &ThisClass::HandleUpdateCooldown);
}

void URPGQuickBarWidget::BindWidgets()
{
	for (int i = 0; i < Slots.Num(); ++i)
	{
		Slots[i] = Cast<URPGSlotWidget>(GetWidgetFromName(FName(FString::Printf(TEXT("QuickBarSlot_%d"), i))));
		check(Slots[i]);
		Slots[i]->SetSlotIndex(i);
		Slots[i]->SetSlotType(ESlotType::Quick);
	}
	
}

void URPGQuickBarWidget::HandleUpdateQuickBarSlot(FGameplayTag Tag, const FUpdateQuickBarSlotMsg& Msg)
{
	if (Slots[Msg.SlotIndex])
	{
		Slots[Msg.SlotIndex]->UpdateSlot(Msg.Thumbnail, Msg.Count);
		Slots[Msg.SlotIndex]->UpdateTooltipContent(Msg.TooltipContent);
	}
		
}

void URPGQuickBarWidget::HandleUpdateCooldown(FGameplayTag Tag, const FUpdateQuickBarCooldownMsg& Msg)
{
	if (Slots[Msg.SlotIndex])
	{
		Slots[Msg.SlotIndex]->UpdateCooldown(Msg.RemainingTime, Msg.CooldownTime);
	}
		
}

