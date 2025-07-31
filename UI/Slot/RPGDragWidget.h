

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGDragWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGDragWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetIcon(UTexture2D* IconTexture);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;
};
