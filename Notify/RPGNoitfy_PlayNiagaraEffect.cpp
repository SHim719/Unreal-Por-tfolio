// 


#include "RPGNoitfy_PlayNiagaraEffect.h"


#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

#if WITH_EDITOR
#include "Logging/MessageLog.h"
#include "Misc/UObjectToken.h"
#endif

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RPGNoitfy_PlayNiagaraEffect)

/////////////////////////////////////////////////////
// URPGNoitfy_PlayNiagaraEffect

URPGNoitfy_PlayNiagaraEffect::URPGNoitfy_PlayNiagaraEffect()
	: Super()
{
	Attached = true;
	Scale = FVector(1.f);
	bAbsoluteScale = false;

#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(192, 255, 99, 255);
#endif // WITH_EDITORONLY_DATA
}

void URPGNoitfy_PlayNiagaraEffect::PostLoad()
{
	Super::PostLoad();

	RotationOffsetQuat = FQuat(RotationOffset);
}

#if WITH_EDITOR
void URPGNoitfy_PlayNiagaraEffect::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(URPGNoitfy_PlayNiagaraEffect, RotationOffset))
	{
		RotationOffsetQuat = FQuat(RotationOffset);
	}
}

void URPGNoitfy_PlayNiagaraEffect::ValidateAssociatedAssets()
{
	static const FName NAME_AssetCheck("AssetCheck");

	if ((Template != nullptr) && (Template->IsLooping()))
	{
		UObject* ContainingAsset = GetContainingAsset();

		FMessageLog AssetCheckLog(NAME_AssetCheck);

		const FText MessageLooping = FText::Format(
			NSLOCTEXT("AnimNotify", "NiagaraSystem_ShouldNotLoop", "Niagara system {0} used in anim notify for asset {1} is set to looping, but the slot is a one-shot (it won't be played to avoid leaking a component per notify)."),
			FText::AsCultureInvariant(Template->GetPathName()),
			FText::AsCultureInvariant(ContainingAsset->GetPathName()));
		AssetCheckLog.Warning()
			->AddToken(FUObjectToken::Create(ContainingAsset))
			->AddToken(FTextToken::Create(MessageLooping));

		if (GIsEditor)
		{
			AssetCheckLog.Notify(MessageLooping, EMessageSeverity::Warning, /*bForce=*/ true);
		}
	}
}
#endif


void URPGNoitfy_PlayNiagaraEffect::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//Store the spawned effect in a protected variable
	SpawnedEffect = SpawnEffect(MeshComp, Animation);
	
	//Call to BP to allows setting of Niagara User Variables
	Super::Notify(MeshComp, Animation, EventReference);
}

FString URPGNoitfy_PlayNiagaraEffect::GetNotifyName_Implementation() const
{
	if (Template)
	{
		return Template->GetName();
	}
	else
	{
		return Super::GetNotifyName_Implementation();
	}
}

UFXSystemComponent* URPGNoitfy_PlayNiagaraEffect::SpawnEffect(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UFXSystemComponent* ReturnComp = nullptr;

	if (Template)
	{
		if (Template->IsLooping())
		{
			return ReturnComp;
		}

		if (Attached)
		{
			EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset;
			if (bUseActorWorldOffset)
			{
				LocationType = EAttachLocation::KeepWorldPosition;
			}
			
			ReturnComp = UNiagaraFunctionLibrary::SpawnSystemAttached(Template, MeshComp, SocketName, LocationOffset, RotationOffset, EAttachLocation::KeepRelativeOffset, true);
		}
		else
		{
			const FTransform MeshTransform = MeshComp->GetSocketTransform(SocketName);
			FVector SpawnLocation = MeshTransform.TransformPosition(LocationOffset);
			if (bUseActorWorldOffset)
			{
				FRotator ActorRotator = MeshComp->GetOwner()->GetActorRotation();
				FVector Right = ActorRotator.Quaternion().GetRightVector();
				FVector Up = ActorRotator.Quaternion().GetUpVector();
				FVector Look = ActorRotator.Quaternion().GetForwardVector();
				SpawnLocation = MeshTransform.GetLocation() + Right * LocationOffset.Y + Up * LocationOffset.Z + Look * LocationOffset.X;
			}
			
			ReturnComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp->GetWorld(), Template, SpawnLocation, (MeshTransform.GetRotation() * RotationOffsetQuat).Rotator(), FVector(1.0f),true);
		}

		if (ReturnComp != nullptr)
		{
			ReturnComp->SetUsingAbsoluteScale(bAbsoluteScale);
			ReturnComp->SetRelativeScale3D_Direct(Scale);
		}
	}

	return ReturnComp;
}

UFXSystemComponent* URPGNoitfy_PlayNiagaraEffect::GetSpawnedEffect() 
{
	return SpawnedEffect;
}

