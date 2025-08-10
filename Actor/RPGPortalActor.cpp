#include "RPGPortalActor.h"

#include "RPGHelperLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelChange/RPGLevelChangeManager.h"


ARPGPortalActor::ARPGPortalActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(SphereComponent);
}

void ARPGPortalActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGPortalActor::Interact()
{
	if (auto* PlayerDataManager = RPGHelper::GetPlayerDataManager(this))
	{
		PlayerDataManager->SaveCurrentPlayerData();
	}

	if (auto* LevelChangeManager = RPGHelper::GetLevelChangeManager(this))
	{
		LevelChangeManager->ReserveLevelChange(PlayerStartTag, ChangeLevelName);
	}
}





