#include "Character/RPGCharacterBase.h"

#include "MotionWarpingComponent.h"
#include "RPGHelperLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectPool/RPGPoolManager.h"
#include "UI/DamageText/RPGDamageText.h"


ARPGCharacterBase::ARPGCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	
	DamageNumberPositionComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DamageNumberPosComp"));
	DamageNumberPositionComponent->SetupAttachment(GetRootComponent());

	GetMesh()->SetReceivesDecals(false);
	GetCapsuleComponent()->SetReceivesDecals(false);
}

void ARPGCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityInfo();
}

void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	
}

void ARPGCharacterBase::ShowDamageNumberText(const float Damage, const bool bIsCritical) const
{
	auto* PoolManager = RPGHelper::GetPoolManager(this);
	if (PoolManager == nullptr)
		return;

	if (URPGDamageText* DamageTextWidget = Cast<URPGDamageText>(PoolManager->GetPoolObject("DamageNumber")))
	{
		if (const auto* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			FVector2D WorldToScreen = FVector2D::Zero();

			if (PC->ProjectWorldLocationToScreen(DamageNumberPositionComponent->GetComponentLocation(), WorldToScreen))
			{
				const FVector2D RandomOffset(FMath::FRandRange(-20.f, 20.f), FMath::FRandRange(-20.f, 20.f));
				WorldToScreen = WorldToScreen + RandomOffset;
				DamageTextWidget->SetDamage(Damage, bIsCritical);
				DamageTextWidget->SetPositionInViewport(WorldToScreen);
			}
		}
	}
	
}


UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARPGCharacterBase::OnHit(const FGameplayEventData* Payload)
{
}


void ARPGCharacterBase::Die()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
 	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//Dissolve();
}

void ARPGCharacterBase::InitAbilityInfo()
{
	
}








