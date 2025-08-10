#include "RPGPlayerDataManager.h"

#include "AbilitySystemComponent.h"
#include "RPGHelperLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Character/RPGPlayerCharacter.h"
#include "Components/RPGEquipmentComponent.h"
#include "Components/RPGQuickBarComponent.h"
#include "Components/RPGSkillTreeComponent.h"

void URPGPlayerDataManager::SaveCurrentPlayerData()
{
	AbilityPoints.Empty();
	InventoryDatas.Empty();
	EquipmentDatas.Empty();
	QuickBarRefSlotInfoDatas.Empty();
	CooldownInfos.Empty();
	
	if (ARPGPlayerState* PS = RPGHelper::GetPlayerState_Safe(this))
	{
		if (auto* InventoryComponent = PS->FindComponentByClass<URPGInventoryComponent>())
		{
			InventoryComponent->GetItemEntryDatasForSave(InventoryDatas);
			CurrentGold = InventoryComponent->GetCurrentGold();
		}

		if (auto* EquipmentComponent = PS->FindComponentByClass<URPGEquipmentComponent>())
		{
			EquipmentComponent->GetEquipmentEntryDatas(EquipmentDatas);
		}

		if (auto* QuickBarComponent = PS->FindComponentByClass<URPGQuickBarComponent>())
		{
			QuickBarRefSlotInfoDatas = QuickBarComponent->GetRefSlotInfos();
		}

		if (auto* SkillTreeComponent = PS->FindComponentByClass<URPGSkillTreeComponent>())
		{
			SkillTreeComponent->GetSkillLevels(AbilityPoints);
			SkillPoint = SkillTreeComponent->GetSkillPoint();
		}

		if (auto* ASC = PS->GetAbilitySystemComponent())
		{
			FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(FName("Cooldown"));
			FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
			const TArray<FActiveGameplayEffectHandle> ActiveCooldownHandles = ASC->GetActiveEffects(Query);

			for (const FActiveGameplayEffectHandle& Handle : ActiveCooldownHandles)
			{
				if (const FActiveGameplayEffect* CooldownEffect = ASC->GetActiveGameplayEffect(Handle))
				{
					const float TimeRemaining = CooldownEffect->GetTimeRemaining(GetWorld()->GetTimeSeconds());
		
					FGameplayTagContainer GrantedTags;
					CooldownEffect->Spec.GetAllGrantedTags(GrantedTags);
		
					for (const FGameplayTag& Tag : GrantedTags)
					{
						if (Tag.MatchesTag(CooldownTag))
						{
							CooldownInfos.Add(Tag, TimeRemaining);
							break; 
						}
					}
				}
			}
		}

		if (auto* AttributeSet = PS->GetAttributeSet())
		{
			Hp = AttributeSet->GetHP();
			Exp = AttributeSet->GetXP();
		}
		

		Level = PS->GetPlayerLevel();
	}
	
}

void URPGPlayerDataManager::LoadCurrentPlayerData()
{
	if (ARPGPlayerState* PS = RPGHelper::GetPlayerState_Safe(this))
	{
		PS->Initialize(Level);

		if (auto* AttributeSet = PS->GetAttributeSet())
		{
			if (Hp >= 0)
			{
				AttributeSet->SetHP(Hp);
			}
			else
			{
				AttributeSet->SetHP(AttributeSet->GetMaxHP());
			}
			
			if (Exp >= 0)
			{
				AttributeSet->SetXP(Exp);
			}
		}

		if (auto* InventoryComponent = PS->FindComponentByClass<URPGInventoryComponent>())
		{
			InventoryComponent->Initialize(InventoryDatas, CurrentGold);
		}

		if (auto* EquipmentComponent = PS->FindComponentByClass<URPGEquipmentComponent>())
		{
			EquipmentComponent->Initialize(EquipmentDatas);
		}

		if (auto* QuickBarComponent = PS->FindComponentByClass<URPGQuickBarComponent>())
		{
			QuickBarComponent->Initialize(QuickBarRefSlotInfoDatas);
		}

		if (auto* SkillTreeComponent = PS->FindComponentByClass<URPGSkillTreeComponent>())
		{
			SkillTreeComponent->AddSkillPoint(SkillPoint);
		}

		if (auto* ASC = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent()))
		{
			for (auto& Pair : AbilityPoints)
			{
				ASC->SetAbilityLevel(Pair.Key, Pair.Value);
			}
		}
		
	}
}
