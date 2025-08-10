

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "RPGEnemy.generated.h"

struct FRPGEnemyDataRow;
struct FOnAttributeChangeData;
class ULevelSequence;
class UGameplayEffect;
class UBehaviorTree;
class UWidgetComponent;


UCLASS()
class RPGPROJECT_API ARPGEnemy : public ARPGCharacterBase
{
	GENERATED_BODY()

public:
	ARPGEnemy();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	virtual void OnHit(const FGameplayEventData* Payload) override;
	virtual void Die() override;

	void UpdateWarpTargetPosition();

protected:
	void InitAttributes();
	void BindAttributeDelegate();

	virtual UUserWidget* GetHpBarWidget() const;
	void BindHpBarViewModel(UUserWidget* HpBarWidget) const;

	virtual void HandleHpChanged(const FOnAttributeChangeData& Data);

	void StartBehaviorTree();
protected:
	void BroadcastKillEnemyMsg();
	void DropItems() const;
	void GiveExpToPlayer() const;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitVFX();

	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy|Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Weapon")
	FName WeaponAttachSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	FName EnemyId;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> InitAttributesEffect;

	UPROPERTY(EditDefaultsOnly)
	float DropRadius = 50.f;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 5.f;
};
