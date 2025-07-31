

#pragma once

#include "GameplayEffectTypes.h"
#include "RPGAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FRPGGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
 
public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
 
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
 	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	virtual FGameplayEffectContext* Duplicate() const override
	{
		FGameplayEffectContext* NewContext = new FGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
 
 
	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)	override;
 	
protected:
	UPROPERTY()
	bool bIsCriticalHit = false;
 	
};

template<>
struct TStructOpsTypeTraits<FRPGGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FRPGGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};