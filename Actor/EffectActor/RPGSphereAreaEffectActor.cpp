#include "RPGSphereAreaEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RPGHelperLibrary.h"
#include "Components/SphereComponent.h"


ARPGSphereAreaEffectActor::ARPGSphereAreaEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComponent->SetCollisionResponseToChannel(ECC_Terrain, ECR_Ignore);
	SetRootComponent(SphereComponent);
}


void ARPGSphereAreaEffectActor::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);
	
}

void ARPGSphereAreaEffectActor::Destroyed()
{
	Super::Destroyed();

	//for (auto& Pair : ActiveEffects)
	//{
	//	RemoveEffectFromTarget(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(), Pair.Value);
	//}
}


bool ARPGSphereAreaEffectActor::ApplyEffectToTarget(AActor* OtherActor)
{
	if (OtherActor == nullptr)
		return false;

	if (RPGHelper::IsFriend(OtherActor, GetEffectCauser()) == true)
		return false;

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		FRPGActiveEffectSpecHandles ActiveSpecHandles;
		ActiveSpecHandles.ActiveEffectHandles.Reserve(EffectSpecHandles.Num());
		for (auto& EffectEffectSpecHandle : EffectSpecHandles)
		{
			if (EffectEffectSpecHandle.IsValid())
			{
				ActiveSpecHandles.ActiveEffectHandles.Add(TargetASC->ApplyGameplayEffectSpecToSelf(*EffectEffectSpecHandle.Data.Get()));
			}
		}
		ActiveEffects.Add(OtherActor->GetUniqueID(), ActiveSpecHandles);
	}

	return true;
}

void ARPGSphereAreaEffectActor::RemoveEffectFromTarget(UAbilitySystemComponent* ASC, FRPGActiveEffectSpecHandles& ActiveEffectHandles)
{
	if (ASC == nullptr)
		return;

	for (FActiveGameplayEffectHandle& ActiveSpecHandle : ActiveEffectHandles.ActiveEffectHandles)
	{
		if (ActiveSpecHandle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(ActiveSpecHandle);
			ActiveSpecHandle.Invalidate();
		}
	}
}


void ARPGSphereAreaEffectActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || ActiveEffects.Contains(OtherActor->GetUniqueID()))
		return;

	ApplyEffectToTarget(OtherActor);
	
}

void ARPGSphereAreaEffectActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr || ActiveEffects.Contains(OtherActor->GetUniqueID()) == false)
		return;

	const int32 ActorID = OtherActor->GetUniqueID();

	RemoveEffectFromTarget(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor), ActiveEffects[ActorID]);

	ActiveEffects.Remove(ActorID);
}





