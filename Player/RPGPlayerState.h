

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "RPGPlayerState.generated.h"

class URPGProcComponent;
class URPGAttributeSet;
class URPGAbilitySystemComponent;
class UGameplayEffect;
class URPGJobData;
class URPGConsumeComponent;
class URPGQuickBarComponent;
class URPGSkillTreeComponent;
class URPGEquipmentComponent;
class URPGInventoryComponent;


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeLevel, const int32 NewLevel, bool bLevelUp)

UCLASS()
class RPGPROJECT_API ARPGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARPGPlayerState();
	void Initialize(int32 Level);

protected:
	void BindAttributeDelegates() const;
	void UpdateDefaultAttributes() const;

public:
	void GiveExp(const int32 Exp) const;
	void LevelUp(const int32 Count);
	void SetLevel(const int32 NewLevel, bool bLevelUp);
	
	FOnChangeLevel OnChangeLevel;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	URPGAttributeSet* GetAttributeSet() const { return AttributeSet;}
	
	int32 GetPlayerLevel() const { return PlayerLevel;}
	
	
protected:
	int32 PlayerLevel = 0;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URPGJobData>			JobData;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect>	InitDefaultAttributesEffect;
	
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<URPGAbilitySystemComponent>  AbilitySystemComponent;
 
	UPROPERTY()
	TObjectPtr<URPGAttributeSet>			AttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<URPGInventoryComponent>		InventoryComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URPGEquipmentComponent>		EquipmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<URPGSkillTreeComponent>		SkillTreeComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URPGQuickBarComponent>		QuickBarComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URPGConsumeComponent>		ConsumeComponent;

	UPROPERTY()
	TObjectPtr<URPGProcComponent>			ProcComponent;

};
