

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Slot/RPGSlotWidget.h"
#include "RPGGridSlots.generated.h"


class UUniformGridSlot;
class UUniformGridPanel;

UCLASS()
class RPGPROJECT_API URPGGridSlots : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	void HideAllSlots();
	void UpdateSlotCount(const int32 SlotCount);
	
	void SetSlotType(const ESlotType Type);
	void ClearAllSlots();
	
	URPGSlotWidget* GetSlotWidget(const int32 Index) { return SlotWidgets[Index]; }

private:
	void AddSlots();
	void AddSlotToGrid(const int32 Index, URPGSlotWidget* ItemSlot);
	void SetSlotRowCol(UUniformGridSlot* GridSlot, const int32 Index) const;
	

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<URPGSlotWidget> SlotWidgetClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	int32 SlotsPerRow = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	int32 TotalSlots = 0;

	UPROPERTY(EditAnywhere)
	bool bCanDragDrop = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<TObjectPtr<URPGSlotWidget>> SlotWidgets;

	UPROPERTY()
	TArray<TObjectPtr<UUniformGridSlot>> GridSlots; // 화면에 렌더링될 슬롯의 순서 정보를 저장한다.
	
};
