

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_ProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMVVM_ProgressBar : public UMVVMViewModelBase
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	float CurrentValue;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	float MaxValue;

public:
	void SetCurrentValue(float NewCurrentValue)
	{
		if (UE_MVVM_SET_PROPERTY_VALUE(CurrentValue, NewCurrentValue))
		{
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetValuePercent);
		}
	}	
	
	void SetMaxValue(float NewMaxValue)
	{
		if (UE_MVVM_SET_PROPERTY_VALUE(MaxValue, NewMaxValue))
		{
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetValuePercent);
		}
	}
	
	float GetCurrentValue() const
	{
		return CurrentValue;
	}
	
	float GetMaxValue() const
	{
		return MaxValue;
	}

public:
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetValuePercent() const
	{
		if (MaxValue != 0)
		{
			return CurrentValue / MaxValue;
		}
		else
			return 0.f;
	}
		
	
};
