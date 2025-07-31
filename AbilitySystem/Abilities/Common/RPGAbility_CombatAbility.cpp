#include "RPGAbility_CombatAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RPGHelperLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/RPGCharacterBase.h"
#include "Character/Enemy/RPGEnemy.h"
#include "Kismet/GameplayStatics.h"


void URPGAbility_CombatAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (bPlayMontageOnActivated)
	{
		ActivateMontageTask();
	}
	
	if (bIsChanneling)
	{
		GetWorld()->GetTimerManager().SetTimer(ChannelingTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::OnChannelingEnd),
			GetChannelingTime(), false);
	}

	if (bLookToMouse && IsPlayer() == true)
	{
		LookToMouse();
	}

	if (IsPlayer() == false)
	{
		ARPGEnemy* OwningEnemy = Cast<ARPGEnemy>(GetOwningActorFromActorInfo());
		if (OwningEnemy == nullptr)
			return;
		
		if (bUpdateFacingTargetOnActivated == true)
		{
			OwningEnemy->UpdateWarpTargetPosition();
		}

		if (auto* Blackboard = RPGHelper::GetBlackboardComponent_Safe(OwningEnemy))
		{
			Blackboard->SetValueAsBool(FName("IsEngaged"), true);
		}
	}
	
}

void URPGAbility_CombatAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (bIsChanneling)
	{
		GetWorld()->GetTimerManager().ClearTimer(ChannelingTimerHandle);
		ChannelingTimerHandle.Invalidate();
	}

	if (IsPlayer() == false)
	{
		ARPGEnemy* OwningEnemy = Cast<ARPGEnemy>(GetOwningActorFromActorInfo());
		if (OwningEnemy == nullptr)
			return;
		
		if (auto* Blackboard = RPGHelper::GetBlackboardComponent_Safe(OwningEnemy))
		{
			Blackboard->SetValueAsBool(FName("IsEngaged"), false);
		}
	}
}

void URPGAbility_CombatAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (bCancelOnInputReleased)
	{
		K2_EndAbility();
	}
}

FGameplayEffectSpecHandle URPGAbility_CombatAbility::MakeEffectSpecHandle(const FRPGGameplayEffectDef& ApplyEffectDef) const
{
	if (ApplyEffectDef.EffectClass == nullptr)
		return FGameplayEffectSpecHandle();

	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.AddInstigator(GetOwningActorFromActorInfo(), GetOwningActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(ApplyEffectDef.EffectClass, 0, EffectContextHandle);
	
	if (FGameplayEffectSpec* Spec = EffectSpecHandle.Data.Get())
	{
		for (const FGameplayTag& Tag : ApplyEffectDef.SetByCallerTagContainer)
		{
			Spec->SetSetByCallerMagnitude(Tag, GetAttributeDataFromCurveTableByTag(Tag));

			if (Tag.MatchesTagExact(PeriodTag))
			{
				Spec->Period = GetAttributeDataFromCurveTableByTag(PeriodTag);
			}
		}
	}

	return EffectSpecHandle;
}


float URPGAbility_CombatAbility::GetAttributeDataFromCurveTableByTag(const FGameplayTag& Tag) const
{
	if (AbilityAttributeTable == nullptr)
		return 0.f;
	
	if (const FRealCurve* AttributeDataCurve = AbilityAttributeTable->FindCurve(Tag.GetTagName(), "Query AttributeDataCurve"))
	{
		const float Value = AttributeDataCurve->Eval(GetAbilityLevel());
		return Value;
	}

	return 0.f;
}

void URPGAbility_CombatAbility::OnChannelingEnd()
{
	K2_EndAbility();
}

float URPGAbility_CombatAbility::GetChannelingTime() const
{
	return URPGAbility_CombatAbility::GetAttributeDataFromCurveTableByTag(ChannelingTimeTag);
}

void URPGAbility_CombatAbility::GetEffectSpecHandles(TArray<FGameplayEffectSpecHandle>& EffectSpecHandles) const
{
	EffectSpecHandles.Reserve(ApplyEffectDefs.Num());
	for (const FRPGGameplayEffectDef& EffectDef : ApplyEffectDefs)
	{
		EffectSpecHandles.Add(MakeEffectSpecHandle(EffectDef));
	}
}


void URPGAbility_CombatAbility::ActivateMontageTask()
{
	if (Montage)
	{
		MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, Montage, 1.f);
		if (MontageTask)
		{
			MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
			MontageTask->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
			MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
			MontageTask->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);
			MontageTask->ReadyForActivation();
		}
	}
}


bool URPGAbility_CombatAbility::IsPlayer() const
{
	AActor* OwningActor = GetOwningActorFromActorInfo();
	
	return OwningActor->ActorHasTag(FName("Player"));
}

void URPGAbility_CombatAbility::LookToMouse() const
{
	if (auto* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		FHitResult Hit;
		if (true == PC->GetHitResultUnderCursor(ECC_Terrain, false, Hit))
		{
			if (AActor* OwnerActor = GetOwningActorFromActorInfo())
			{
				const FVector LookDirection = (Hit.ImpactPoint - OwnerActor->GetActorLocation()).GetSafeNormal2D();
				OwnerActor->SetActorRotation(LookDirection.Rotation());
			}
			
		}
	}
	
}



