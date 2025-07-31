#include "RPGLevelWidget.h"

#include "RPGHelperLibrary.h"
#include "Components/TextBlock.h"
#include "Player/RPGPlayerState.h"

void URPGLevelWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ARPGPlayerState* PlayerState = RPGHelper::GetPlayerState_Safe(this))
	{
		PlayerState->OnChangeLevel.AddUObject(this, &ThisClass::ChangeLevelText);
	}
}

void URPGLevelWidget::ChangeLevelText(const int32 NewLevel, bool bLevelUp) const
{
	if (Text_Level)
	{
		Text_Level->SetText(FText::AsNumber(NewLevel));
	}
	
}
