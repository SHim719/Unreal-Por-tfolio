#include "RPGProjectileActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "RPGHelperLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ARPGProjectileActor::ARPGProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComponent->SetCollisionProfileName(FName("Projectile"));
	RootComponent = SphereComponent;
	
}

void ARPGProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	if (SphereComponent)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
	}
}

void ARPGProjectileActor::OnExitPool()
{
	Super::OnExitPool();

	GetGameInstance()->GetTimerManager().SetTimer(LifeSpanTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::OnReturnToPool), LifeSpan, false);
	CurrentHitCount = 0;
}

void ARPGProjectileActor::ReturnToPool()
{
	Super::ReturnToPool();

	GetGameInstance()->GetTimerManager().ClearTimer(LifeSpanTimerHandle);
	LifeSpanTimerHandle.Invalidate();
}



void ARPGProjectileActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ApplyEffectToTarget(OtherActor) == true)
	{
		CurrentHitCount++;
		if (CurrentHitCount == HitCount)
		{
			ReturnToPool();
		}
	}

	if (OtherActor != GetEffectCauser())
	{
		HitImpact();
	}
}

void ARPGProjectileActor::ResetVelocity()
{
	if (ProjectileMovementComponent)
	{
		const FVector InitialVelocity = FVector(ProjectileMovementComponent->InitialSpeed, 0.f, 0.f);
		ProjectileMovementComponent->SetVelocityInLocalSpace(InitialVelocity);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}


