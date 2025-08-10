
#include "Character/RPGPlayerCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "RPGPlayerCaptureCharacter.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/RPGEquipMeshManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/RPGInteractionComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Interface/InteractableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RPGPlayerState.h"


ARPGPlayerCharacter::ARPGPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->bDoCollisionTest = false;

	InteractionSphereComponent = CreateDefaultSubobject<USphereComponent>("InteractionSphereComponent");
	InteractionSphereComponent->SetupAttachment(RootComponent);
	InteractionSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	InteractionComponent = CreateDefaultSubobject<URPGInteractionComponent>(TEXT("InteractionComp"));
	EquipMeshManagerComponent = CreateDefaultSubobject<URPGEquipMeshManagerComponent>(TEXT("EquipMeshManagerComp"));
}



void ARPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerCaptureCharacterClass)
	{
		GetWorld()->SpawnActor(PlayerCaptureCharacterClass);
	}
	
	InteractionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnInteractionSphereBeginOverlap);
	InteractionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnInteractionSphereEndOverlap);

}

void ARPGPlayerCharacter::InitAbilityInfo()
{
	if (ARPGPlayerState* PS = Cast<ARPGPlayerState>(GetPlayerState()))
	{
		AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AttributeSet = PS->GetAttributeSet();

		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->InitAbilityActorInfo(this, this);
		}

		PS->OnChangeLevel.AddUObject(this, &ThisClass::OnLevelChanged);
	}
}


void ARPGPlayerCharacter::Die()
{
	//Super::Die();
}

void ARPGPlayerCharacter::OnLevelChanged(const int32 NewLevel,  bool bLevelUp) const
{
	if (bLevelUp == true && LevelUpVfx && LevelUpSound)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, LevelUpVfx, GetActorLocation());
		UGameplayStatics::PlaySound2D(this, LevelUpSound);
	}
	
}



void ARPGPlayerCharacter::OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (InteractionComponent && OtherActor->Implements<UInteractableInterface>())
	{
		InteractionComponent->UpdateInteractableObject(OtherActor);
	}
}

void ARPGPlayerCharacter::OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InteractionComponent && OtherActor->Implements<UInteractableInterface>())
	{
		InteractionComponent->UpdateInteractableObject(OtherActor);
	}
}
