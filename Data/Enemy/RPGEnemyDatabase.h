

#pragma once

#include "CoreMinimal.h"
#include "Data/Item/RPGDropItemInfo.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGEnemyDatabase.generated.h"

class UGameplayAbility;
class URPGGameplayAbility;

USTRUCT()
struct FRPGEnemyDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly)
	float Hp;

	UPROPERTY(EditDefaultsOnly)
	float Attack;

	UPROPERTY(EditDefaultsOnly)
	float Defense;

	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly)
	float Xp;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> GrantAbilities;

	UPROPERTY(EditDefaultsOnly)
	TArray<FRPGDropItemInfo> DropItems;
};

USTRUCT()
struct FRPGEnemyDamageAttributeRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<float> BaseDamages;

	UPROPERTY(EditDefaultsOnly)
	TArray<float> AttackCoefficients;
};



UCLASS()
class RPGPROJECT_API URPGEnemyDatabase : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	const FRPGEnemyDataRow* GetEnemyData(const FName& EnemyId) const;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> EnemyTable;
	
};
