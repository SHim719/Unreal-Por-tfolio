

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGMainWidget.generated.h"

class URPGTooltipManager;
class URPGSkillTreeWidget;
class URPGInventoryWidget;

UCLASS()
class RPGPROJECT_API URPGMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	void ToggleWidgetVisibility(const int32 WidgetIndex);


private:
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> Widgets;
	
};
