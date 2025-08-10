// 


#include "RPGFadeScreen.h"

#include "Animation/UMGSequencePlayer.h"

void URPGFadeScreen::StartFade(const EFadeType FadeType)
{
	if (FadeType == EFadeType::FadeIn)
	{
		if (UUMGSequencePlayer* SequencePlayer = PlayAnimation(FadeIn, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, true))
		{
			SequencePlayer->OnSequenceFinishedPlaying().AddUObject(this, &ThisClass::OnAnimationSequenceFinished);
		}
	}
	else
	{
		if (UUMGSequencePlayer* SequencePlayer = PlayAnimation(FadeOut, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, true))
		{
			SequencePlayer->OnSequenceFinishedPlaying().AddUObject(this, &ThisClass::OnAnimationSequenceFinished);
		}
	}
}

void URPGFadeScreen::OnAnimationSequenceFinished(UUMGSequencePlayer& Player)
{
	OnFadeEndedDelegate.ExecuteIfBound();
	RemoveFromParent();
}


