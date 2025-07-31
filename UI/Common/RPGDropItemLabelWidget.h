// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGDropItemLabelWidget.generated.h"


DECLARE_DELEGATE(FOnClickItemLabel)

class UTextBlock;

UCLASS()
class RPGPROJECT_API URPGDropItemLabelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	void SetLabelText(const FText& InText, const int32 ItemCount);
	
	FOnClickItemLabel OnClickItemLabel;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_ItemLabel;
	
};
