// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPGConsumeAttribute.generated.h"

class ARPGPlayerCharacter;
class UGameplayEffect;

UCLASS(Abstract, Const, DefaultToInstanced, EditInlineNew, CollapseCategories)
class RPGPROJECT_API URPGConsumeAttribute : public UObject
{
	GENERATED_BODY()

public:
	virtual bool Consume(AActor* Player) PURE_VIRTUAL(virtual bool Consume(AActor* Player), return true;)
	virtual FText GetTooltipText(const UObject* WorldContextObject) PURE_VIRTUAL(virtual FText GetTooltipText(), return FText::GetEmpty();)

protected:
	void PlayConsumeSound(UObject* WorldContextObject) const;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle OptionRowHandle;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SoundWhenConsumed;
};

