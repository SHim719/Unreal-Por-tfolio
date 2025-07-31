

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "RPGAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)		\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)	\
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)				\
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)				\
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


#define IS_RPGATTRIBUTE(RPGAttribute)\
Attribute == Get##RPGAttribute##_BaseAttribute() || \
Attribute == Get##RPGAttribute##_BonusFlatAttribute() || \
Attribute == Get##RPGAttribute##_BonusPercentAttribute()

#define HANDLE_RPGATTRIBUTE(RPGAttribute) \
float New##RPGAttribute = Get##RPGAttribute##_Base() + Get##RPGAttribute##_BonusFlat(); \
New##RPGAttribute += New##RPGAttribute * Get##RPGAttribute##_BonusPercent() / 100.f; \
Set##RPGAttribute(New##RPGAttribute); \

UCLASS()
class RPGPROJECT_API URPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	URPGAttributeSet();
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)	override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	void HandleIncomingXpChanged();
	void HandleMoveSpeedChanged();
	
	void ShowDamageNumberText(const float Damage, const bool IsCritical) const;
	
	
public:
	UPROPERTY()
	FGameplayAttributeData HP;
	ATTRIBUTE_ACCESSORS(ThisClass, HP);
	
	UPROPERTY() 
	FGameplayAttributeData MaxHP;
	UPROPERTY() 
	FGameplayAttributeData MaxHP_Base;
	UPROPERTY()
	FGameplayAttributeData MaxHP_BonusFlat;
	UPROPERTY()
	FGameplayAttributeData MaxHP_BonusPercent;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHP); 
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHP_Base); 
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHP_BonusFlat); 
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHP_BonusPercent);
	
	UPROPERTY()
	FGameplayAttributeData XP;
	ATTRIBUTE_ACCESSORS(ThisClass, XP);
	
	UPROPERTY()
	FGameplayAttributeData MaxXP;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxXP);

	UPROPERTY() 
	FGameplayAttributeData Attack;
	UPROPERTY() 
	FGameplayAttributeData Attack_Base;
	UPROPERTY()
	FGameplayAttributeData Attack_BonusFlat;
	UPROPERTY()
	FGameplayAttributeData Attack_BonusPercent;
	ATTRIBUTE_ACCESSORS(ThisClass, Attack); 
	ATTRIBUTE_ACCESSORS(ThisClass, Attack_Base); 
	ATTRIBUTE_ACCESSORS(ThisClass, Attack_BonusFlat); 
	ATTRIBUTE_ACCESSORS(ThisClass, Attack_BonusPercent);


	UPROPERTY() 
	FGameplayAttributeData Defense;
	UPROPERTY() 
	FGameplayAttributeData Defense_Base;
	UPROPERTY()
	FGameplayAttributeData Defense_BonusFlat;
	UPROPERTY()
	FGameplayAttributeData Defense_BonusPercent;
	ATTRIBUTE_ACCESSORS(ThisClass,Defense); 
	ATTRIBUTE_ACCESSORS(ThisClass,Defense_Base); 
	ATTRIBUTE_ACCESSORS(ThisClass,Defense_BonusFlat); 
	ATTRIBUTE_ACCESSORS(ThisClass,Defense_BonusPercent);

	UPROPERTY() 
	FGameplayAttributeData CriticalRate;
	UPROPERTY() 
	FGameplayAttributeData CriticalRate_Base;
	UPROPERTY()
	FGameplayAttributeData CriticalRate_BonusFlat;
	UPROPERTY()
	FGameplayAttributeData CriticalRate_BonusPercent;
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalRate); 
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalRate_Base); 
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalRate_BonusFlat); 
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalRate_BonusPercent);

	UPROPERTY() 
	FGameplayAttributeData MoveSpeed;
	UPROPERTY() 
	FGameplayAttributeData MoveSpeed_Base;
	UPROPERTY()
	FGameplayAttributeData MoveSpeed_BonusFlat;
	UPROPERTY()
	FGameplayAttributeData MoveSpeed_BonusPercent;
	ATTRIBUTE_ACCESSORS(ThisClass, MoveSpeed); 
	ATTRIBUTE_ACCESSORS(ThisClass, MoveSpeed_Base); 
	ATTRIBUTE_ACCESSORS(ThisClass, MoveSpeed_BonusFlat); 
	ATTRIBUTE_ACCESSORS(ThisClass, MoveSpeed_BonusPercent);

	UPROPERTY()
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(ThisClass, IncomingDamage);

	UPROPERTY()
	FGameplayAttributeData IncomingExp;
	ATTRIBUTE_ACCESSORS(ThisClass, IncomingExp);
};
