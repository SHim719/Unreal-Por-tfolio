// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "RPGShopWidget.generated.h"

struct FGoldChangedMsg;
class UVerticalBox;
class URPGShopEntryWidget;
class UButton;
class UTextBlock;


UCLASS()
class RPGPROJECT_API URPGShopWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const FName& ShopId);
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


private:
	UFUNCTION()
	void OnClicked_BuyButton();

	void OnFocusEntry(const int32 EntryIndex);
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPGShopEntryWidget> ShopEntryClass;


private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_ShopEntries;

	UPROPERTY()
	TArray<TObjectPtr<URPGShopEntryWidget>> ShopEntries;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_BuyItem;


	int32 CurrentFocusEntryIndex = 0;
};
