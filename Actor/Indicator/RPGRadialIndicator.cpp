#include "RPGRadialIndicator.h"

#include "Components/DecalComponent.h"


ARPGRadialIndicator::ARPGRadialIndicator()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>("DefaultComp");
	RootComponent = DefaultSceneComponent;

	DecalComponent = CreateDefaultSubobject<UDecalComponent>("Decal");
	DecalComponent->SetupAttachment(RootComponent);
}

void ARPGRadialIndicator::Initialize_Implementation(const float Radius, const float Angle, const float FillAmount)
{
}

void ARPGRadialIndicator::UpdateFillAmount_Implementation(const float FillAmount)
{
}






