// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGLoadingScreen.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGLoadingScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	void OnLoadingComplete();
	
};
