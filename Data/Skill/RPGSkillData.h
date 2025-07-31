

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"

#include "RPGSkillData.generated.h"


class UGameplayAbility;


USTRUCT()
struct FRPGSkillDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FText SkillName;

	UPROPERTY(EditDefaultsOnly)
	FText SkillDescription;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> MainSkillIcon;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> SkillAbility;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveTable> SkillAttributeTable;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag MainSkillTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CooldownTag;
 };

USTRUCT()
struct FRPGSkillTreeNode
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 Row = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Col = 0;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SkillTag;

	UPROPERTY(EditDefaultsOnly)
	int32 MasterLevel;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, int32> RequiredSkills;
	
};


UCLASS()
class RPGPROJECT_API URPGSkillTreeData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FRPGSkillTreeNode> SkillNodes;


	int32 FindMaxColumn() const;
	
};


UCLASS()
class RPGPROJECT_API URPGSkillDatabase : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	const FRPGSkillDataRow* GetSkillDataByName(const FName& Name) const;
	const FRPGSkillDataRow* GetSkillDataByTag(const FGameplayTag& Tag) const;
private:
	UPROPERTY()
	TObjectPtr<UDataTable> SkillTable;
	
};
