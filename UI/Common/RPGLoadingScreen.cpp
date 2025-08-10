// 


#include "RPGLoadingScreen.h"

#include "RPGHelperLibrary.h"
#include "LevelChange/RPGLevelChangeManager.h"

void URPGLoadingScreen::NativeConstruct()
{
	Super::NativeConstruct();

	FTimerHandle LoadingTimer;
	GetWorld()->GetTimerManager().SetTimer(LoadingTimer, FTimerDelegate::CreateUObject(this, &ThisClass::OnLoadingComplete), 2.f, false);
}

void URPGLoadingScreen::OnLoadingComplete()
{
	if (auto* LevelChangeManager = RPGHelper::GetLevelChangeManager(this))
	{
		LevelChangeManager->ConfirmLevelChange();
	}
}
