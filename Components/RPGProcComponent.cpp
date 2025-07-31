#include "RPGProcComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RPGHelperLibrary.h"


URPGProcComponent::URPGProcComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
    
}

void URPGProcComponent::Init()
{
    OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
}


void URPGProcComponent::AddProcEffect(const FRPGProcEffectInstance& ProcEffectInstance)
{
	ProcEffectInstances.Add(ProcEffectInstance);

	if (OwnerASC)
	{
	    if (ProcEventHandles.Contains(ProcEffectInstance.TriggerTag) == false)
	    {
	        FDelegateHandle Handle = OwnerASC->GenericGameplayEventCallbacks.FindOrAdd(ProcEffectInstance.TriggerTag).AddUObject(this, &ThisClass::HandleGameplayEvent);
	        ProcEventHandles.Add(ProcEffectInstance.TriggerTag, Handle);
	    }
	}
}

void URPGProcComponent::RemoveProcEffect(const FRPGProcEffectInstance& RemoveProcEffect)
{
    ProcEffectInstances.RemoveSingle(RemoveProcEffect);

    // 태그 이벤트 수신 여부 확인
    bool bIsEventStillRemain = false;
    for (const auto& ProcEffect : ProcEffectInstances)
    {
        if (ProcEffect.TriggerTag == RemoveProcEffect.TriggerTag)
        {
            bIsEventStillRemain = true;
            break;
        }
    }

    // 더 이상 필요없다면 이벤트 수신 중단
    if (bIsEventStillRemain == false && OwnerASC)
    {
        if (FDelegateHandle* Handle = ProcEventHandles.Find(RemoveProcEffect.TriggerTag))
        {
            OwnerASC->GenericGameplayEventCallbacks.Remove(RemoveProcEffect.TriggerTag);
            ProcEventHandles.Remove(RemoveProcEffect.TriggerTag);
        }
    }
}

void URPGProcComponent::HandleGameplayEvent(const FGameplayEventData* Payload)
{
    if (Payload == nullptr || Payload->EventTag.IsValid() == false)
        return;
    
    for (const FRPGProcEffectInstance& ProcEffect : ProcEffectInstances)
    {
        if (ProcEffect.TriggerTag == Payload->EventTag)
        {
            // 확률 계산 실패
            if (FMath::FRand() * 100.f > ProcEffect.ChancePercentage)
            {
                continue; 
            }

            if (ProcEffect.ApplyEffectSpecHandle.IsValid())
            {
                if (ProcEffect.Target == ERPGProcTarget::Self && OwnerASC != nullptr)
                {
                    OwnerASC->ApplyGameplayEffectSpecToSelf(*ProcEffect.ApplyEffectSpecHandle.Data.Get());
                }
                else if (Payload->TargetData.Num())
                {
                    for (auto& Target : Payload->TargetData.Data)
                    {
                        if (Target)
                        {
                            Target->ApplyGameplayEffectSpec(*ProcEffect.ApplyEffectSpecHandle.Data.Get());
                        }
                    }
                }
            }
        }
    }
}


