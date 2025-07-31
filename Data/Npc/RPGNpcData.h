// 

#pragma once

#include "CoreMinimal.h"
#include "Data/Dialogue/RPGDialogueData.h"
#include "UObject/Object.h"
#include "RPGNpcData.generated.h"


UENUM()
enum class ENpcType : uint8
{
	Default,
	Quest,
	Shop,
	End UMETA(Hidden),
};

USTRUCT()
struct FRPGNpcDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	ENpcType NpcType = ENpcType::Default;

	UPROPERTY(EditDefaultsOnly)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly)
	FText InitialDialogue;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "NpcType == ENpcType::Quest", EditConditionHides))
	TArray<FName> GivableQuestIds;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "NpcType == ENpcType::Shop", EditConditionHides))
	FName ShopId;
	
};