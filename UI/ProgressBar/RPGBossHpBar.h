// 

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/RPGProgressBar.h"
#include "RPGBossHpBar.generated.h"

class ARPGEnemy;
class UTextBlock;
class UProgressBar;

UCLASS()
class RPGPROJECT_API URPGBossHpBar : public URPGProgressBar
{
	GENERATED_BODY()

public:
	void SetBossNameText(FText BossName) const;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_BossName;
};
