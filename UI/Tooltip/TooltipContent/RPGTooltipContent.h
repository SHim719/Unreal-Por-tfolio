// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "RPGTooltipContent.generated.h"

class URPGSkillInstance;
class URPGItemInstance;

UENUM()
enum class ERPGTooltipType
{
	Default,
	Item,
	Consume,
	Equipment,
	EquipmentSample,
	Skill,
	End,
};


UCLASS()
class RPGPROJECT_API URPGTooltipContent : public UObject
{
	GENERATED_BODY()
	
public:
	ERPGTooltipType TooltipType = ERPGTooltipType::Default;

	FText Description;
};


UCLASS()
class RPGPROJECT_API URPGItemTooltipContent : public URPGTooltipContent
{
	GENERATED_BODY()
	
public: 
	URPGItemTooltipContent() { TooltipType = ERPGTooltipType::Item; }
	
	FName ItemId;
};

UCLASS()
class URPGEquipmentTooltipContent : public URPGItemTooltipContent
{
	GENERATED_BODY()
public:
	URPGEquipmentTooltipContent() { TooltipType = ERPGTooltipType::Equipment; }
	
	UPROPERTY()
	const URPGItemInstance* ItemInstance = nullptr;
};

UCLASS()
class URPGSkillTooltipContent : public URPGTooltipContent
{
	GENERATED_BODY()
	
public:
	URPGSkillTooltipContent() { TooltipType = ERPGTooltipType::Skill; }
	
	UPROPERTY()
	const URPGSkillInstance* SkillInstance = nullptr;
};

