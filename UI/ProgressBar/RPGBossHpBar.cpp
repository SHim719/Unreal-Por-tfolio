#include "RPGBossHpBar.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Character/Enemy/RPGEnemy.h"
#include "Components/TextBlock.h"


void URPGBossHpBar::SetBossNameText(FText BossName) const
{
	if (Text_BossName)
	{
		Text_BossName->SetText(BossName);
	}
}
