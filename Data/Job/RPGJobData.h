

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RPGJobData.generated.h"

class URPGSkillTreeData;
class UGameplayAbility;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGJobData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> LevelStatSettingEffect;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URPGSkillTreeData> SkillTreeData;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> GrantDefaultAbilities;
};
