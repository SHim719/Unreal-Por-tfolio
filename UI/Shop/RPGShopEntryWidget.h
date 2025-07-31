// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGShopEntryWidget.generated.h"

class UImage;
class UButton;
class URPGSlotWidget;
class UTextBlock;

DECLARE_DELEGATE_OneParam(FOnFocusEntry, const int32 Index)


UCLASS()
class RPGPROJECT_API URPGShopEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void Init(const FName& ItemId, const int32 EntryIndex);
	void ReleaseFocus();
	
	FName GetRefItemId() const { return RefItemId;}
	FOnFocusEntry OnFocusEntryDelegate;

private:
	UFUNCTION()
	void FocusThisEntry();


private:
	FName RefItemId;
	int32 CurrentEntryIndex = 0;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor OriginColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor FocusColor;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URPGSlotWidget> ItemEntry;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Price;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Focus;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Background;

};
