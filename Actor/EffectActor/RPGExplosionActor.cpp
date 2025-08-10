#include "RPGExplosionActor.h"

#include "Actor/Indicator/RPGRadialIndicator.h"
#include "Engine/OverlapResult.h"


ARPGExplosionActor::ARPGExplosionActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ARPGExplosionActor::Initialize()
{
	RadialIndicator = Cast<ARPGRadialIndicator>(GetWorld()->SpawnActor(RadialIndicatorClass));
	if (RadialIndicator)
	{
		RadialIndicator->Initialize(AffectRadius, AffectAngle, 0.f);
		RadialIndicator->SetActorLocation(GetActorLocation());
	}
}


void ARPGExplosionActor::OnReturnToPool()
{
	Super::OnReturnToPool();

	if (RadialIndicator)
	{
		RadialIndicator->Destroy();
		RadialIndicator = nullptr;
	}
	
	CurTime = 0.f;
}

void ARPGExplosionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurTime += DeltaTime;
	if (CurTime >= ExplosionDelay)
	{
		QueryAffectRange();
		ShowExplodeEffect();
		CurTime = 0.f;
		ReturnToPool();
	}
	else
	{
		RadialIndicator->UpdateFillAmount(CurTime / ExplosionDelay);
	}
}

void ARPGExplosionActor::ShowExplodeEffect_Implementation()
{
}

void ARPGExplosionActor::QueryAffectRange()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionObjectQueryParams CollisionObjectParams;
	CollisionObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	if (GetWorld()->OverlapMultiByObjectType(OverlapResults, GetActorLocation(), GetActorRotation().Quaternion()
		,CollisionObjectParams, FCollisionShape::MakeSphere(AffectRadius)) == true)
	{
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			ApplyEffectToTarget(OverlapResult.GetActor());
		}
	}
}

