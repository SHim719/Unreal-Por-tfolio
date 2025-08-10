#include "RPGBossEnemy.h"
#include "RPGHelperLibrary.h"
#include "Data/Enemy/RPGEnemyDatabase.h"
#include "Sequence/RPGSequenceManager.h"
#include "UI/ProgressBar/RPGBossHpBar.h"


ARPGBossEnemy::ARPGBossEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
	
}


void ARPGBossEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (Sequence)
	{
		StartSequence();
	}
	else
	{
		StartBossBattle();
	}
}

UUserWidget* ARPGBossEnemy::GetHpBarWidget() const
{
	return RPGHelper::GetWidgetInMainWidgetFromName(this, FName("BossHpBar"));
}

void ARPGBossEnemy::Die()
{
	Super::Die();

	if (auto* BossHpBar = Cast<URPGBossHpBar>(GetHpBarWidget()))
	{
		BossHpBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ARPGBossEnemy::StartSequence()
{
	if (URPGSequenceManager* SequenceManager = RPGHelper::GetSequenceManager(this))
	{
		SequenceManager->PlaySequence(Sequence);
		SequenceManager->OnSequenceFinished.AddUObject(this, &ThisClass::StartBossBattle);
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ARPGBossEnemy::StartBossBattle()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	StartBehaviorTree();

	if (URPGBossHpBar* HpBar = Cast<URPGBossHpBar>(GetHpBarWidget()))
	{
		if (const FRPGEnemyDataRow* EnemyData = RPGHelper::GetEnemyDataRow_Safe(this, EnemyId))
		{
			HpBar->SetVisibility(ESlateVisibility::HitTestInvisible);
			HpBar->SetBossNameText(EnemyData->DisplayName);
		}
	}
}


