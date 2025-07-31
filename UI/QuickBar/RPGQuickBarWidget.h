

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "RPGQuickBarWidget.generated.h"

struct FUpdateQuickBarCooldownMsg;
struct FUpdateQuickBarSlotMsg;
class UTextBlock;
class URPGSlotWidget;

UCLASS()
class RPGPROJECT_API URPGQuickBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	void BindWidgets();
	void HandleUpdateQuickBarSlot(FGameplayTag Tag, const FUpdateQuickBarSlotMsg& Msg);
	void HandleUpdateCooldown(FGameplayTag Tag, const FUpdateQuickBarCooldownMsg& Msg);

private:
	UPROPERTY()
	TArray<TObjectPtr<URPGSlotWidget>> Slots;

	UPROPERTY()
	TArray<TObjectPtr<UTextBlock>> SlotKeyTextBlocks;
};
