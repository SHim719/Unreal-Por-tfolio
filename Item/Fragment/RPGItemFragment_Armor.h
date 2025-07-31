

#pragma once

#include "CoreMinimal.h"
#include "RPGItemFragment_Attachable.h"
#include "RPGItemFragment_Armor.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGItemFragment_Armor : public URPGItemFragment_Attachable
{
	GENERATED_BODY()

public:
	URPGItemFragment_Armor();

	
public:
	UPROPERTY(EditDefaultsOnly)
	EArmorType ArmorType = EArmorType::End;
	
};
