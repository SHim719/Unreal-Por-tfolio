

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Interface/InteractableInterface.h"
#include "RPGNpcBase.generated.h"

struct FEndNpcInteractionMsg;
struct FRPGNpcDataRow;
class UWidgetComponent;
class USphereComponent;

UCLASS()
class RPGPROJECT_API ARPGNpcBase : public ACharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:
	ARPGNpcBase();
	virtual void BeginPlay() override;
	virtual void Interact() override;

protected:
	virtual void HandleEndInteraction(FGameplayTag Tag, const FEndNpcInteractionMsg& Msg);
	
	void InitNpcDescWidget();
	

protected:
	UPROPERTY(EditDefaultsOnly)
	FName NpcId;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> NpcDescWidgetComponent;
	
	UPROPERTY()
	FGameplayMessageListenerHandle EndInteractionMsgHandler;
	
};
