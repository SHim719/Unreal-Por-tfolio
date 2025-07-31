#include "AbilitySystem/RPGAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "RPGAbilityTypes.h"
#include "RPGGameplayTags.h"
#include "RPGHelperLibrary.h"
#include "Character/RPGCharacterBase.h"
#include "Character/RPGPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/RPGPlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RPGAttributeSet)


URPGAttributeSet::URPGAttributeSet()
{

}

void URPGAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (FMath::IsNearlyEqual(OldValue, NewValue))
		return;
	
	if (IS_RPGATTRIBUTE(Attack))
	{
		HANDLE_RPGATTRIBUTE(Attack);
	}
	else if (IS_RPGATTRIBUTE(Defense))
	{
		HANDLE_RPGATTRIBUTE(Defense);
	}
	else if (IS_RPGATTRIBUTE(MaxHP))
	{
		HANDLE_RPGATTRIBUTE(MaxHP);
	}
	else if (IS_RPGATTRIBUTE(CriticalRate))
	{
		HANDLE_RPGATTRIBUTE(CriticalRate);
	}
	else if (IS_RPGATTRIBUTE(MoveSpeed))
	{
		HANDLE_RPGATTRIBUTE(MoveSpeed);
	}
	else if (Attribute == GetMoveSpeedAttribute())
	{
		HandleMoveSpeedChanged();
	}
	else if (Attribute == GetIncomingExpAttribute())
	{
		HandleIncomingXpChanged();
	}
	
}

void URPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	auto* ASC = GetOwningAbilitySystemComponent();

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			float NewHP = FMath::Clamp(GetHP() - LocalIncomingDamage, 0.f, GetMaxHP());
			SetHP(NewHP);
			
			if (NewHP == 0.f)
			{
				if (ARPGCharacterBase* RPGCharacter = Cast<ARPGCharacterBase>(GetOwningAbilitySystemComponent()->GetOwnerActor()))
				{
					RPGCharacter->Die();
				}
			}
			
			bool IsCriticalHit = false;
			FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
			if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
			{
				IsCriticalHit = EffectContext->IsCriticalHit();
			}
			
			ShowDamageNumberText(LocalIncomingDamage, IsCriticalHit);
		}
	}

}

void URPGAttributeSet::HandleIncomingXpChanged()
{
	if (ARPGPlayerState* PlayerState = Cast<ARPGPlayerState>(GetOwningActor()))
	{
		float IncomingXp = GetIncomingExp();
		float CurrentMaxXp = GetMaxXP();
		while (IncomingXp >= CurrentMaxXp)
		{
			PlayerState->LevelUp(1);
			IncomingXp = FMath::Max(IncomingXp - CurrentMaxXp, 0);
			CurrentMaxXp = GetMaxXP();
		}
		SetXP(IncomingXp);
		IncomingExp.SetCurrentValue(0);
	}
}

void URPGAttributeSet::HandleMoveSpeedChanged()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	if (OwnerCharacter == nullptr)
	{
		OwnerCharacter = RPGHelper::GetPlayerCharacter_Safe(GetOwningActor());
	}
	
	if (auto* CharacterMovement = OwnerCharacter->GetCharacterMovement())
	{
		CharacterMovement->MaxWalkSpeed = GetMoveSpeed();
	}
	
}

void URPGAttributeSet::ShowDamageNumberText(const float Damage, const bool bIsCritical) const
{
	if (ARPGCharacterBase* Character = Cast<ARPGCharacterBase>(GetOwningAbilitySystemComponent()->GetOwnerActor()))
	{
		Character->ShowDamageNumberText(Damage, bIsCritical);
	}

}
