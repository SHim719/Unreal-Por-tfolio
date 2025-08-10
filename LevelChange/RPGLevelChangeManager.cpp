#include "RPGLevelChangeManager.h"

#include "Kismet/GameplayStatics.h"
#include "UI/Common/RPGFadeScreen.h"


bool URPGLevelChangeManager::ShouldCreateSubsystem(UObject* Outer) const
{
	if (this->GetClass()->IsInBlueprint() && Super::ShouldCreateSubsystem(Outer))
		return true;

	return false;
}

void URPGLevelChangeManager::ReserveLevelChange(FName DestPlayerStartTag, FName DestLevel)
{
	PlayerStartTag = DestPlayerStartTag;
	ToChangeLevel = DestLevel;
	
	//UGameplayStatics::SetGamePaused(this, true);

	if (auto* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (URPGFadeScreen* FadeScreen = CreateWidget<URPGFadeScreen>(PC, FadeScreenClass))
		{
			FadeScreen->StartFade(EFadeType::FadeOut);
			FadeScreen->OnFadeEndedDelegate.BindLambda([this, PC]()
			{
				if (auto* LoadingWidget = CreateWidget(PC, LoadingWidgetClass))
				{
					LoadingWidget->AddToViewport();
				}
			});

			FadeScreen->AddToViewport();
		}
	}
	
}

void URPGLevelChangeManager::ConfirmLevelChange()
{
	//UGameplayStatics::SetGamePaused(this, false);
	UGameplayStatics::OpenLevel(this, ToChangeLevel);
}
