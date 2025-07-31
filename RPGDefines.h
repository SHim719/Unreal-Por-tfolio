#pragma once
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"

#include "RPGDefines.generated.h"

#define ECC_Terrain ECollisionChannel::ECC_GameTraceChannel1

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Equipment				UMETA(DisplayName = "Equipment"),
	Consume					UMETA(DisplayName = "Consume"),
	Etc						UMETA(DisplayName = "Etc"),
	Gold					UMETA(DisplayName = "Gold"),
	Exp						UMETA(DisplayName = "Exp"),
	End						UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Armor	 UMETA(DisplayName	= "Armor"),
	Weapon   UMETA(DisplayName	= "Weapon"),
	End		 UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Sword	UMETA(DisplayName = "Sword"),
	Wand	UMETA(DisplayName = "Wand"),
	End		UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	Helmets UMETA(DisplayName = "Helmet"),
	Top		UMETA(DisplayName = "Top"),
	Bottom  UMETA(DisplayName = "Bottom"),
	Shoes	UMETA(DisplayName = "Shoes"),
	End		UMETA(Hidden)
};

enum class ESlotType : uint8
{
	Inventory_Equip,
	Inventory_Consume,
	Inventory_Etc,
	Equip,
	Skill,
	Quick,
	SellItem,
	End,
};


UENUM(BlueprintType)
enum class EEquipSlotType : uint8
{
	Helmets, 
	Top,
	Bottom,
	Shoes,
	Weapon,
	End		
};

UENUM()
enum class ERPGWidgetType : uint8
{
	Inventory,
	SkillTree,
	Quest,
	End,
};


USTRUCT()
struct FHitQueryDefine
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float AttackAngle = 0.f;
};









