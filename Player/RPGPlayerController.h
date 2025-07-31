#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPGPlayerController.generated.h"


class ARPGTargetCircle;
class URPGAbilitySystemComponent;
class URPGGameplayAbility;
class UInputAction;
class UInputMappingContext;

UCLASS()
class RPGPROJECT_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARPGPlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void BindInputs();

	ARPGTargetCircle* ShowTargetCircle(const float Radius);
	void HideTargetCircle() const;

private:
	void BindMouseInputMove(UEnhancedInputComponent* EnhancedInputComponent);
	void BindQuickBarInput(UEnhancedInputComponent* EnhancedInputComponent);
	void BindChangeVisibilityWidgetInput(UEnhancedInputComponent* EnhancedInputComponent);
	void BindInteractionInput(UEnhancedInputComponent* EnhancedInputComponent);
	
private:
	void ChangeVisibilityWidget(const int32 WidgetIndex);
	void HandleMovement(bool bKeyInputStarted);
	void DefaultAttack();
	void Dash();
	void ConfirmTarget();

private:
	UPROPERTY()
	TObjectPtr<URPGAbilitySystemComponent> OwnerASC;

	UPROPERTY()
	TObjectPtr<ARPGTargetCircle> TargetCircle;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARPGTargetCircle> TargetCircleClass;

private:
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> SetDestinationClickAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> BasicAttackAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> DashAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TArray<TObjectPtr<UInputAction>> ChangeVisibilityWidgetActions;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TArray<TObjectPtr<UInputAction>> QuickSlotInputActions;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> ConfirmTargetAction;
	
};
