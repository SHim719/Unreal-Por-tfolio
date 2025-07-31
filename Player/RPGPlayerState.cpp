


#include "Player/RPGPlayerState.h"

#include "RPGHelperLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Components/RPGConsumeComponent.h"
#include "Components/RPGEquipmentComponent.h"
#include "Components/RPGInventoryComponent.h"
#include "Components/RPGQuickBarComponent.h"
#include "Components/RPGSkillTreeComponent.h"
#include "Data/Job/RPGJobData.h"
#include "UI/RPGHUD.h"
#include "UI/ViewModel/MVVM_ProgressBar.h"
#include "GameplayEffect.h"
#include "Components/RPGProcComponent.h"


ARPGPlayerState::ARPGPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));
	
	InventoryComponent = CreateDefaultSubobject<URPGInventoryComponent>(TEXT("InventoryComp"));
	EquipmentComponent = CreateDefaultSubobject<URPGEquipmentComponent>(TEXT("EquipmentComp"));
	SkillTreeComponent = CreateDefaultSubobject<URPGSkillTreeComponent>(TEXT("SkillTreeComp"));
	QuickBarComponent = CreateDefaultSubobject<URPGQuickBarComponent>(TEXT("QuickBarComp"));
	ConsumeComponent = CreateDefaultSubobject<URPGConsumeComponent>(TEXT("ConsumeComp"));
	ProcComponent = CreateDefaultSubobject<URPGProcComponent>(TEXT("ProcComponent"));
}

void ARPGPlayerState::Initialize(const int32 Level)
{
	if (JobData)
	{
		SkillTreeComponent->InitializeSkillTreeByData(JobData->SkillTreeData);
		InitDefaultAttributesEffect = JobData->LevelStatSettingEffect;

		for (auto AbilityClass : JobData->GrantDefaultAbilities)
		{
			AbilitySystemComponent->K2_GiveAbility(AbilityClass);
		}
	}
	
	BindAttributeDelegates();

	SetLevel(Level, false);

	ProcComponent->Init();
}



UAbilitySystemComponent* ARPGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ARPGPlayerState::GiveExp(const int32 Exp) const
{
	if (AttributeSet)
	{
		AttributeSet->SetIncomingExp(Exp);
	}

}

void ARPGPlayerState::LevelUp(const int32 Count) 
{
	SkillTreeComponent->AddSkillPoint(5 * Count);
	SetLevel(PlayerLevel + Count, true);
}

void ARPGPlayerState::SetLevel(const int32 NewLevel, bool bLevelUp)
{
	const int32 OldLevel = PlayerLevel;
	PlayerLevel = NewLevel;
	UpdateDefaultAttributes();
	OnChangeLevel.Broadcast(NewLevel, bLevelUp);
}

void ARPGPlayerState::BindAttributeDelegates() const
{
	if (ARPGHUD* HUD = RPGHelper::GetHUD_Safe(this))
	{
		HUD->VM_GlobeHpBar->SetMaxValue(AttributeSet->GetMaxHP());
		HUD->VM_GlobeHpBar->SetCurrentValue(AttributeSet->GetHP());
		HUD->VM_ExpBar->SetMaxValue(AttributeSet->GetMaxXP());
		HUD->VM_ExpBar->SetCurrentValue(AttributeSet->GetXP());
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHPAttribute()).AddLambda(
			[this, HUD](const FOnAttributeChangeData& Data)
			{
				if (!FMath::IsNearlyEqual(Data.OldValue, Data.NewValue))
					HUD->VM_GlobeHpBar->SetCurrentValue(Data.NewValue);
			}
		);
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHPAttribute()).AddLambda(
			[this, HUD](const FOnAttributeChangeData& Data)
			{
				if (!FMath::IsNearlyEqual(Data.OldValue, Data.NewValue))
					HUD->VM_GlobeHpBar->SetMaxValue(Data.NewValue);
			}
		);
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetXPAttribute()).AddLambda(
		[this, HUD](const FOnAttributeChangeData& Data)
		{
			if (!FMath::IsNearlyEqual(Data.OldValue, Data.NewValue))
				HUD->VM_ExpBar->SetCurrentValue(Data.NewValue);
		}
	);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxXPAttribute()).AddLambda(
			[this, HUD](const FOnAttributeChangeData& Data)
			{
				if (!FMath::IsNearlyEqual(Data.OldValue, Data.NewValue))
					HUD->VM_ExpBar->SetMaxValue(Data.NewValue);
			}
		);
	}
}

void ARPGPlayerState::UpdateDefaultAttributes() const
{
	if (InitDefaultAttributesEffect == nullptr || AbilitySystemComponent == nullptr)
		return;
	
	const FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle InitAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InitDefaultAttributesEffect, PlayerLevel, EffectContextHandle);

	if (FGameplayEffectSpec* Spec = InitAttributesSpecHandle.Data.Get())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
	}

	if (AttributeSet)
	{
		AttributeSet->SetHP(AttributeSet->GetMaxHP());
	}
	
}






