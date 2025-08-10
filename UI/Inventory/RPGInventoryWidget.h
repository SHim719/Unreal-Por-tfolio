

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "UI/Common/RPGMovableWidget.h"
#include "RPGInventoryWidget.generated.h"

struct FGoldChangedMsg;
class URPGEquipmentWidget;
class UTextBlock;
class URPGInventorySlots;
class URPGIndexedButton;

UCLASS()
class RPGPROJECT_API URPGInventoryWidget : public URPGMovableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	
private:
	void BindWidgets();

	UFUNCTION()
	void HandleInventoryTabClicked(int32 ButtonIndex);
	
	void HandleChangedGoldMsg(FGameplayTag, const FGoldChangedMsg& Msg);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URPGEquipmentWidget> EquipmentWidget;
	
	UPROPERTY()
	TArray<TObjectPtr<URPGIndexedButton>> InventoryTabButtons;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URPGInventorySlots> InventorySlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldText;

};
