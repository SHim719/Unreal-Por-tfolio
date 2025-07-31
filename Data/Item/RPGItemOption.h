

#pragma once


#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/ProcSystem/ProcSystemTypes.h"
#include "RPGItemOption.generated.h"

class UGameplayAbility;

UENUM()
enum class ERPGItemOptionType : uint8
{
	PassiveStatModifer,
	ProcEffect,
};


USTRUCT()
struct FRPGValueRange
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ValueTag;

	UPROPERTY(EditDefaultsOnly)
	int32 MinValue = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxValue = 0;
	
};

USTRUCT()
struct FRPGItemOptionRandomValueInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle OptionDataRowHandle;

	UPROPERTY(EditDefaultsOnly)
	TArray<FRPGValueRange> OptionValueRanges;
};

USTRUCT()
struct FRPGItemOptionInstance
{
	GENERATED_BODY()
	
	UPROPERTY()
	FDataTableRowHandle OptionDataRowHandle;
	
	TArray<TPair<FGameplayTag, int32>> OptionValues;
};


USTRUCT()
struct FRPGItemOptionRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	FText OptionText;
	
	UPROPERTY(EditDefaultsOnly)
	ERPGItemOptionType OptionType;

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "OptionType == ERPGItemOptionType::PassiveStatModifer", EditConditionHides))
	TSubclassOf<UGameplayEffect> ToApplyStatModifierEffect;

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "OptionType == ERPGItemOptionType::ProcEffect", EditConditionHides))
	FRPGProcEffectDefinition ProcEffectDef;
	
};

