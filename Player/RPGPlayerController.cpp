#include "Player/RPGPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RPGPlayerState.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Actor/RPGTargetCircle.h"
#include "Components/RPGInteractionComponent.h"
#include "Components/RPGQuickBarComponent.h"
#include "GameFramework/HUD.h"
#include "UI/RPGHUD.h"
#include "UI/RPGMainWidget.h"


ARPGPlayerController::ARPGPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerASC = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPlayerState<ARPGPlayerState>()));
	check(OwnerASC);

	TargetCircle = GetWorld()->SpawnActor<ARPGTargetCircle>(TargetCircleClass);
	HideTargetCircle();
	
	BindInputs();
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
}

void ARPGPlayerController::BindInputs()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		BindMouseInputMove(EnhancedInputComponent);
		BindQuickBarInput(EnhancedInputComponent);
		BindChangeVisibilityWidgetInput(EnhancedInputComponent);
		BindInteractionInput(EnhancedInputComponent);

		if (BasicAttackAction)
		{
			EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Started, this, &ThisClass::DefaultAttack);
		}

		if (DashAction)
		{
			EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ThisClass::Dash);
		}

		if (ConfirmTargetAction)
		{
			EnhancedInputComponent->BindAction(ConfirmTargetAction, ETriggerEvent::Started, this, &ThisClass::ConfirmTarget);
		}
	}
}

ARPGTargetCircle* ARPGPlayerController::ShowTargetCircle(const float Radius)
{
	if (TargetCircle)
	{
		TargetCircle->ShowTargetCircle(Radius);
	}

	return TargetCircle;
}

void ARPGPlayerController::HideTargetCircle() const
{
	if (TargetCircle)
	{
		TargetCircle->HideTargetCircle();
	}
}


void ARPGPlayerController::BindMouseInputMove(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (SetDestinationClickAction == nullptr)
		return;

	URPGQuickBarComponent* QuickBarComponent = GetPlayerState<APlayerState>()->FindComponentByClass<URPGQuickBarComponent>();

	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ThisClass::HandleMovement, true);
	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed,this, &ThisClass::HandleMovement, false);
}
	


void ARPGPlayerController::BindQuickBarInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (URPGQuickBarComponent* QuickBarComponent = GetPlayerState<APlayerState>()->FindComponentByClass<URPGQuickBarComponent>())
	{
		for (int i = 0; i < QuickSlotInputActions.Num(); ++i)
		{
			if (QuickSlotInputActions[i])
			{
				EnhancedInputComponent->BindAction(QuickSlotInputActions[i], ETriggerEvent::Started, QuickBarComponent, &URPGQuickBarComponent::KeyInputStarted, i);
				EnhancedInputComponent->BindAction(QuickSlotInputActions[i], ETriggerEvent::Completed, QuickBarComponent, &URPGQuickBarComponent::KeyInputReleased, i);
			}
		}
	}
}

void ARPGPlayerController::BindChangeVisibilityWidgetInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	for (int32 i = 0; i < static_cast<int32>(ERPGWidgetType::End); ++i)
	{
		if (ChangeVisibilityWidgetActions[i])
		{
			EnhancedInputComponent->BindAction(ChangeVisibilityWidgetActions[i], ETriggerEvent::Started, this, &ThisClass::ChangeVisibilityWidget, i);
		}
	}
}

void ARPGPlayerController::BindInteractionInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InteractAction == nullptr)
		return;
	
	if (URPGInteractionComponent* InteractionComponent = GetPawn()->FindComponentByClass<URPGInteractionComponent>())
	{
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, InteractionComponent, &URPGInteractionComponent::Interact);
	}
}

void ARPGPlayerController::ChangeVisibilityWidget(const int32 WidgetIndex)
{
	if (ARPGHUD* HUD = Cast<ARPGHUD>(GetHUD()))
	{
		if (auto* MainWidget = HUD->GetMainWidget())
		{
			MainWidget->ToggleWidgetVisibility(WidgetIndex);
		}
	}
}

void ARPGPlayerController::HandleMovement(bool bKeyInputStarted)
{
	if (OwnerASC == nullptr)
		return;

	FGameplayTag AbilityPlayerMovementTag = FGameplayTag::RequestGameplayTag(FName("Ability.PlayerMovement"));
	if (bKeyInputStarted == true)
	{
		OwnerASC->AbilityInputPressedByAssetTag(AbilityPlayerMovementTag);
	}
	else
	{
		OwnerASC->AbilityInputReleasedByAssetTag(AbilityPlayerMovementTag);
	}
}


void ARPGPlayerController::DefaultAttack()
{
	if (OwnerASC)
	{
		OwnerASC->AbilityInputPressedByAssetTag(FGameplayTag::RequestGameplayTag(FName("Skill.DefaultAttack")));
	}
	
}

void ARPGPlayerController::Dash()
{
	if (OwnerASC)
	{
		OwnerASC->AbilityInputPressedByAssetTag(FGameplayTag::RequestGameplayTag(FName("Skill.Dodge")));
	}
}

void ARPGPlayerController::ConfirmTarget()
{
	if (TargetCircle && TargetCircle->IsActive())
	{
		TargetCircle->Confirm();
	}
	
}




