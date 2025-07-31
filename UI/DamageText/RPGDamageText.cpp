#include "UI/DamageText/RPGDamageText.h"

#include "Animation/UMGSequencePlayer.h"
#include "Components/TextBlock.h"


void URPGDamageText::SetDamage(const float Damage, const bool bIsCritical) 
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.MaximumFractionalDigits = 0;
	FormattingOptions.UseGrouping = false;
	Text_Damage->SetText(FText::AsNumber(Damage, &FormattingOptions));
	Text_Damage->SetColorAndOpacity(bIsCritical == true ? CriticalColor : NormalColor);

	if (DamageAnimation)
	{
		if (UUMGSequencePlayer* SequencePlayer = PlayAnimation(DamageAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, true))
		{
			SequencePlayer->OnSequenceFinishedPlaying().AddUObject(this, &ThisClass::OnAnimationSequenceFinished);
		}
	}
}

void URPGDamageText::OnAnimationSequenceFinished(UUMGSequencePlayer& Player)
{
	SetVisibility(ESlateVisibility::Collapsed);
	ReturnToPool();
}
