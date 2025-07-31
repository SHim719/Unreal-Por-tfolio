#include "RPGPlayerCaptureCharacter.h"

#include "RPGHelperLibrary.h"
#include "Actor/RPGEquipmentActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Item/Fragment/RPGItemFragment_Armor.h"
#include "Message/RPGGameMessage.h"


void ARPGPlayerCaptureCharacter::HandleOnPlayerEquipmentChange(FGameplayTag, const FOnPlayerChangeEquipmentMsg& Msg)
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

ARPGPlayerCaptureCharacter::ARPGPlayerCaptureCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComp"));
	CaptureComponent->SetupAttachment(RootComponent);
}

void ARPGPlayerCaptureCharacter::BeginPlay()
{
	Super::BeginPlay();

	ChangeEquipmentListener = RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Player.ChangeEquipment")),
			this, &ThisClass::HandleOnPlayerEquipmentChange);
	
	SetActorLocation(SpawnLocation);
}

void ARPGPlayerCaptureCharacter::BeginDestroy()
{
	Super::BeginDestroy();

	ChangeEquipmentListener.Unregister();
}



