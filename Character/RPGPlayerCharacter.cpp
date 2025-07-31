
#include "Character/RPGPlayerCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "RPGHelperLibrary.h"
#include "RPGPlayerCaptureCharacter.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Actor/RPGEquipmentActor.h"
#include "Camera/CameraComponent.h"
#include "Components/RPGEquipmentComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/RPGInteractionComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Interface/InteractableInterface.h"
#include "Item/Fragment/RPGItemFragment_Armor.h"
#include "Kismet/GameplayStatics.h"
#include "Message/RPGGameMessage.h"
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
}



void ARPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InteractionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnInteractionSphereBeginOverlap);
	InteractionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnInteractionSphereEndOverlap);

}

void ARPGPlayerCharacter::Initialize()
{
	RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Player.ChangeEquipment")),
			this, &ThisClass::HandleOnPlayerEquipmentChange);
	
	if (PlayerCaptureCharacterClass)
	{
		GetWorld()->SpawnActor(PlayerCaptureCharacterClass);
	}
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

void ARPGPlayerCharacter::HandleOnPlayerEquipmentChange(FGameplayTag, const FOnPlayerChangeEquipmentMsg& Msg)
{
	if (Msg.EquipItemInstance == nullptr)
		return;

	auto* ItemFragment_Equippable = Msg.EquipItemInstance->FindFragmentByClass<URPGItemFragment_Equippable>();
	if (ItemFragment_Equippable == nullptr)
		return;

	EEquipmentType EquipmentType = ItemFragment_Equippable->EquipmentType;
	
	if (EquipmentType == EEquipmentType::Weapon)
	{
		if (WeaponActor)
		{
			WeaponActor->Destroy();
			WeaponActor = nullptr;
		}

		if (Msg.bIsEquipped)
		{
			auto* ItemFragment_Attachable = Msg.EquipItemInstance->FindFragmentByClass<URPGItemFragment_Attachable>();
			if (ItemFragment_Attachable == nullptr)
				return;
			
			if (ARPGEquipmentActor* NewWeaponActor = GetWorld()->SpawnActorDeferred<ARPGEquipmentActor>(ARPGEquipmentActor::StaticClass(),
				FTransform::Identity, this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
			{
				NewWeaponActor->SetMesh(ItemFragment_Attachable->AttachMesh);
				NewWeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ItemFragment_Attachable->AttachSocketName);
				NewWeaponActor->FinishSpawning(FTransform::Identity);
				WeaponActor = NewWeaponActor;
			}
		}
	}
	else if (EquipmentType == EEquipmentType::Armor)
	{
		const URPGItemFragment_Armor* ItemFragment_Armor = Cast<URPGItemFragment_Armor>(ItemFragment_Equippable);
		if (ItemFragment_Armor == nullptr)
			return;
		
		USkeletalMeshComponent* ArmorSK = FindComponentByTag<USkeletalMeshComponent>(RPGHelper::ArmorTypeToName(ItemFragment_Armor->ArmorType));
		if (ArmorSK == nullptr)
			return;

		USkeletalMesh* ArmorMesh = Msg.bIsEquipped == true ? ItemFragment_Armor->AttachMesh : nullptr;
		ArmorSK->SetSkeletalMesh(ArmorMesh);
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
