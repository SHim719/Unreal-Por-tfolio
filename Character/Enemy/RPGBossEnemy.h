// 

#pragma once

#include "CoreMinimal.h"
#include "RPGEnemy.h"
#include "RPGBossEnemy.generated.h"

UCLASS()
class RPGPROJECT_API ARPGBossEnemy : public ARPGEnemy
{
	GENERATED_BODY()

public:
	ARPGBossEnemy();
	
protected:
	virtual void BeginPlay() override;
	virtual UUserWidget* GetHpBarWidget() const override;
	virtual void Die() override;
	
	void StartSequence();
	void StartBossBattle();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TObjectPtr<ULevelSequence> Sequence;


};
