#include "Skill/RPGSkillInstance.h"

#include "RPGHelperLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Components/RPGSkillTreeComponent.h"
#include "Data/Skill/RPGSkillData.h"


void URPGSkillInstance::Init(const FGameplayTag& SkillTag, URPGAbilitySystemComponent* InOwnerASC)
{
	OwnerASC = InOwnerASC;
	RefSkillTag = SkillTag;
	
	if (const FRPGSkillDataRow* SkillData = RPGHelper::GetSkillData_Safe(this, SkillTag))
	{
		OwnerASC->K2_GiveAbility(SkillData->SkillAbility, -1);
		InitCooldown(SkillData->CooldownTag);
	}
	
}

void URPGSkillInstance::OnInputPressed()
{
	OwnerASC->AbilityInputPressedByAssetTag(RefSkillTag);
}

void URPGSkillInstance::OnInputReleased()
{
	OwnerASC->AbilityInputReleasedByAssetTag(RefSkillTag);
}

int32 URPGSkillInstance::GetCurrentSkillLevel() const
{
	if (OwnerASC)
	{
		return OwnerASC->GetAbilityLevel(RefSkillTag);
	}
	return 0;
}

float URPGSkillInstance::GetCurrentCooldown() const
{
	if (OwnerASC)
	{
		return OwnerASC->GetCurrentCooldown(RefSkillTag);
	}

	return OwnerASC->GetCurrentCooldown(RefSkillTag);
}


const FRPGSkillTreeNode* URPGSkillInstance::GetSkillTreeNode() const
{
	ARPGPlayerState* PS = RPGHelper::GetPlayerState_Safe(this);
	if (PS == nullptr)
		return nullptr;
	
	URPGSkillTreeComponent* SkillTreeComp = PS->FindComponentByClass<URPGSkillTreeComponent>();
	if (SkillTreeComp == nullptr)
		return nullptr;

	return SkillTreeComp->FindSkillTreeNodeByTag(RefSkillTag);
}

void URPGSkillInstance::InitCooldown(const FGameplayTag& InCooldownTag)
{
	if (InCooldownTag.IsValid())
	{
		CooldownTag = InCooldownTag;
		OwnerASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::CooldownTagChanged);
		//OwnerASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::OnActiveEffectAdded);
	}
}

void URPGSkillInstance::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (CooldownTag != InCooldownTag)
		return;
	
	if (NewCount > 0)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.SetTimer(CooldownUpdateTimerHandle, this, &ThisClass::UpdateCooldownTimeRemaining, 0.05f, true);
	}
	else
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.ClearTimer(CooldownUpdateTimerHandle);
		OnCooldownChanged.Broadcast(0.f, CachedCooldownTime, EnrolledQuickSlotIndex);
		CachedCooldownTime = -1.f;
	}
}

void URPGSkillInstance::UpdateCooldownTimeRemaining()
{
	FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
	TArray<float> TimesRemaining = OwnerASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
	if (TimesRemaining.Num() > 0)
	{
		float TimeRemaining = TimesRemaining[0];
		for (int32 i = 0; i < TimesRemaining.Num(); i++)
		{
			if (TimesRemaining[i] > TimeRemaining)
			{
				TimeRemaining = TimesRemaining[i];
			}
		}
		
		if (CachedCooldownTime < 0.f)
		{
			CachedCooldownTime = TimeRemaining;
		}
		
		OnCooldownChanged.Broadcast(TimeRemaining, CachedCooldownTime, EnrolledQuickSlotIndex);	
	}
}

