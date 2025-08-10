#include "RPGEquipMeshManagerComponent.h"

#include "RPGDefines.h"
#include "Actor/RPGEquipmentActor.h"
#include "Character/RPGPlayerCharacter.h"
#include "Item/Fragment/RPGItemFragment_Armor.h"
#include "Item/Fragment/RPGItemFragment_Equippable.h"
#include "Message/RPGGameMessage.h"


URPGEquipMeshManagerComponent::URPGEquipMeshManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


void URPGEquipMeshManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Player.ChangeEquipment")),
		this, &ThisClass::HandleOnPlayerEquipmentChange);
}

void URPGEquipMeshManagerComponent::HandleOnPlayerEquipmentChange(FGameplayTag, const FOnPlayerChangeEquipmentMsg& Msg)
{
	if (Msg.EquipItemInstance == nullptr)
		return;

	auto* ItemFragment_Equippable = Msg.EquipItemInstance->FindFragmentByClass<URPGItemFragment_Equippable>();
	if (ItemFragment_Equippable == nullptr)
		return;

	ACharacter* OwningCharacter = Cast<ACharacter>( GetOwner());
	if (OwningCharacter == nullptr)
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
				FTransform::Identity, OwningCharacter, OwningCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
			{
				NewWeaponActor->SetMesh(ItemFragment_Attachable->AttachMesh);
				NewWeaponActor->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ItemFragment_Attachable->AttachSocketName);
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

		const EArmorType ArmorType = ItemFragment_Armor->ArmorType;
		
		USkeletalMeshComponent* ArmorSK = ArmorSKs[ArmorType];
		if (ArmorSK == nullptr)
			return;

		USkeletalMesh* ArmorMesh = Msg.bIsEquipped == true ? ItemFragment_Armor->AttachMesh : nullptr;
		ArmorSK->SetSkeletalMesh(ArmorMesh);

		if (const FSkeletalMeshes* ModularArmorSKs = ModularSKs.Find(ArmorType))
		{
			for (auto& ModularSK : ModularArmorSKs->SkeletalMeshses)
			{
				ModularSK->SetVisibility(!Msg.bIsEquipped);
			}
		}
	}
}



