

#pragma once

#include "CoreMinimal.h"
#include "RPGDefines.h"
#include "Blueprint/UserWidget.h"
#include "RPGSlotWidget.generated.h"

class URPGTooltipContent;
class URPGTooltipWidget;
class URPGItemInstance;
class URPGDragWidget;
class UImage;
class UTextBlock;
class UTexture2D;


UCLASS()
class RPGPROJECT_API URPGSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	void SetSlotIndex(const int32 Index) { SlotIndex = Index; }
	void SetSlotType(const ESlotType Type) { SlotType = Type;}
	void UpdateSlot(UTexture2D* Thumbnail, const int32 ItemCount) const;
	void UpdateTooltipContent(const URPGTooltipContent* InTooltipContent);
	void UpdateCooldown(float RemainingTime, float CooldownTime);
	void UpdateCooldownText(float RemainingTime) const;
	
	bool IsEmpty() const;

	void UpdateSlotActiveState(bool bActive);
	void SetCanDragDrop(const bool InbCanDragDrop) { bCanDragDrop = InbCanDragDrop;}
	
private:
	void BroadcastSlotInteraction() const;
	void ShowTooltip();
	void HideTooltip();
	
private:
	int32		SlotIndex = 0;
	ESlotType	SlotType = ESlotType::End;
	bool		bSlotActive = false;
	bool		bMouseOnSlot = false;


	UPROPERTY()
	TObjectPtr<const URPGTooltipContent> TooltipContent;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPGDragWidget> DragSlotClass;

	UPROPERTY(EditAnywhere)
	bool		bCanDragDrop = true;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage>		Image_Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock>	Text_Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage>		Image_Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock>	Text_Cooldown;
	
};
