#include "Game/RPGGameMode.h"

#include "RPGGameInstance.h"
#include "RPGHelperLibrary.h"
#include "DropItem/RPGDropItemActor.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "LevelChange/RPGLevelChangeManager.h"
#include "ObjectPool/RPGPoolManager.h"

void ARPGGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (ARPGHUD* HUD = RPGHelper::GetHUD_Safe(this))
	{
		HUD->Initialize();
	}
	
	if (auto* PoolManager = RPGHelper::GetPoolManager(this))
	{
		if (DropItemActorClass)
		{
			PoolManager->CreateNewPool("DropItemActor", DropItemActorClass, 10, 50, true, 1.5f);
		}
		
	}
	
}



void ARPGGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	if (auto* PlayerDataManager = RPGHelper::GetPlayerDataManager(this))
	{
		PlayerDataManager->LoadCurrentPlayerData();
	}
}

AActor* ARPGGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	URPGGameInstance* GameInstance = Cast<URPGGameInstance>(GetGameInstance());
	if (GameInstance == nullptr)
		return nullptr;
	
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	
	for (AActor* PlayerStart : PlayerStarts)
	{
		if (APlayerStart* CurrentPlayerStart = Cast<APlayerStart>(PlayerStart))
		{
			if (URPGLevelChangeManager* LevelChangeManager = GetGameInstance()->GetSubsystem<URPGLevelChangeManager>())
			{
				if (CurrentPlayerStart->PlayerStartTag.ToString().Equals(LevelChangeManager->PlayerStartTag.ToString(), ESearchCase::IgnoreCase)) 
				{
					return CurrentPlayerStart;
				}
			}
		}
	}

	return nullptr;
}
