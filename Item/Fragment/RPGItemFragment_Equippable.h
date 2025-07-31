// 

#pragma once

#include "CoreMinimal.h"
#include "RPGDefines.h"
#include "RPGItemFragment.h"
#include "Data/Item/RPGItemOption.h"
#include "RPGItemFragment_Equippable.generated.h"



UCLASS(Abstract, Const)
class RPGPROJECT_API URPGItemFragment_Equippable : public URPGItemFragment
{
	GENERATED_BODY()

public:
	EEquipmentType EquipmentType;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FRPGItemOptionRandomValueInfo> MainOptionInfos;

	UPROPERTY(EditDefaultsOnly)
	TArray<FRPGItemOptionRandomValueInfo> RandomOptionInfos;

	UPROPERTY(EditDefaultsOnly)
	int32 RandomOptionCount = 0;
};
