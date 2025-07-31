

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "RPGEquipmentWidget.generated.h"

struct FUpdateEquipmentSlotMsg;
class UImage;
class URPGSlotWidget;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGEquipmentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	void BindWidget();
	void RegisterEvents();

	void HandleEquipmentSlotUpdated(FGameplayTag Tag, const FUpdateEquipmentSlotMsg& Msg);

private:
	UPROPERTY()
	TArray<TObjectPtr<URPGSlotWidget>> EquipmentSlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Player;
};
