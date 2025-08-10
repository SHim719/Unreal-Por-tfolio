#include "RPGPlayerCaptureCharacter.h"

#include "Components/RPGEquipMeshManagerComponent.h"
#include "Components/SceneCaptureComponent2D.h"


ARPGPlayerCaptureCharacter::ARPGPlayerCaptureCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComp"));
	CaptureComponent->SetupAttachment(RootComponent);

	EquipMeshManagerComponent = CreateDefaultSubobject<URPGEquipMeshManagerComponent>(TEXT("EquipMeshManagerComp"));
}

void ARPGPlayerCaptureCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation(SpawnLocation);
}

