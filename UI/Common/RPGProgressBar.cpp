
#include "UI/Common/RPGProgressBar.h"

#include "Components/ProgressBar.h"

void URPGProgressBar::UpdateProgressBarPercentage()
{
	if (MaxValue == 0.f)
		return;

	ProgressBar->SetPercent(CurrentValue / MaxValue);
}
