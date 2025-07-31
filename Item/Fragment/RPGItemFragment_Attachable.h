

#pragma once

#include "CoreMinimal.h"
#include "Item/Fragment/RPGItemFragment_Equippable.h"
#include "RPGItemFragment_Attachable.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RPGPROJECT_API URPGItemFragment_Attachable : public URPGItemFragment_Equippable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMesh> AttachMesh;

	UPROPERTY(EditDefaultsOnly)
	FName AttachSocketName;

	UPROPERTY(EditDefaultsOnly)
	FTransform AttachTransform;
	
};
