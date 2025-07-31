#include "RPGPortalActor.h"

#include "RPGHelperLibrary.h"
#include "Components/SphereComponent.h"
#include "Game/RPGGameInstance.h"
#include "Kismet/GameplayStatics.h"


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
	
	UGameplayStatics::OpenLevel(this, ChangeLevelName);

	if (URPGGameInstance* GameInstance = Cast<URPGGameInstance>(GetGameInstance()))
	{
		GameInstance->PlayerStartTag = PlayerStartTag;
	}
}



