

#pragma once

#include "RPGDropItemInfo.generated.h"

USTRUCT()
struct FRPGDropItemInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	FName ItemId;
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DropRate = 1.0f;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MinQuantity = 1;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxQuantity = 1;
};