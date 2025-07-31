// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGLevelWidget.generated.h"

class UTextBlock;


UCLASS()
class RPGPROJECT_API URPGLevelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	void ChangeLevelText(const int32 NewLevel, bool bLevelUp) const;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Level;
};
