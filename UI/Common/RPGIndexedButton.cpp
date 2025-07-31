#include "RPGIndexedButton.h"

#include "Components/Button.h"

void URPGIndexedButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button->OnClicked.AddDynamic(this, &ThisClass::HandleButtonClicked);
}



void URPGIndexedButton::HandleButtonClicked() 
{
	OnButtonClicked.ExecuteIfBound(ButtonIndex);
}
