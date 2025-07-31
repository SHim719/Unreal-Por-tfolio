


#include "Actor/RPGTargetCircle.h"

#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RPGPlayerController.h"

ARPGTargetCircle::ARPGTargetCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	DecalComponent = CreateDefaultSubobject<UDecalComponent>("Decal");
	DecalComponent->SetupAttachment(RootComponent);
}

void ARPGTargetCircle::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<ARPGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
}


void ARPGTargetCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PC.IsValid())
	{
		FHitResult Hit;
		if (PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit))
		{
			SetActorLocation(Hit.ImpactPoint);
		}
	}
}

void ARPGTargetCircle::ShowTargetCircle(const float Radius)
{
	bIsActive = true;
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	
	if (DecalComponent)
	{
		DecalComponent->DecalSize = FVector(1.f, Radius, Radius);
	}

	if (OnConfirmedPosition.IsBound())
	{
		OnConfirmedPosition.Execute(true, FVector::ZeroVector);
		OnConfirmedPosition.Unbind();
	}
}

void ARPGTargetCircle::HideTargetCircle()
{
	bIsActive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	OnConfirmedPosition.Unbind();
}

void ARPGTargetCircle::Confirm()
{
	OnConfirmedPosition.ExecuteIfBound(false, GetActorLocation());
}

