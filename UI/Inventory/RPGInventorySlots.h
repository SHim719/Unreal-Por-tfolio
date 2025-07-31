

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "RPGInventorySlots.generated.h"

struct FUpdateInventorySlotMsg;
struct FInitInventorySizeMsg;

class URPGGridSlots;

UCLASS()
class RPGPROJECT_API URPGInventorySlots : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void ShowSlots(const int32 ItemTypeToInt32);
	
private:
	void BindWidgets();
	void RegisterEvents();

	void HandleUpdateInventorySize(FGameplayTag Tag, const FInitInventorySizeMsg& Msg);
	void HandleUpdateInventorySlot(FGameplayTag Tag, const FUpdateInventorySlotMsg& Msg);

private:
	UPROPERTY()
	TArray<TObjectPtr<URPGGridSlots>> InventorySlots;
	
};
