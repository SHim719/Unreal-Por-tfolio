// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPGQuestPrerequirement.generated.h"


class ARPGPlayerCharacter;

UCLASS(Abstract, Const, DefaultToInstanced, EditInlineNew, CollapseCategories)
class URPGQuestPrerequirement : public UObject
{
	GENERATED_BODY()

public:
	virtual bool IsMetPrerequirement(ARPGPlayerCharacter* PlayerCharacter) const PURE_VIRTUAL(, return false;)
	
};

UCLASS()
class URPGQuestPrerequirement_Level : public URPGQuestPrerequirement
{
	GENERATED_BODY()
	
	virtual bool IsMetPrerequirement(ARPGPlayerCharacter* PlayerCharacter) const override;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Level = 0;
};

UCLASS()
class URPGQuestPrerequirement_Quest : public URPGQuestPrerequirement
{
	GENERATED_BODY()

	virtual bool IsMetPrerequirement(ARPGPlayerCharacter* PlayerCharacter) const override;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> CompletedQuests;
};




