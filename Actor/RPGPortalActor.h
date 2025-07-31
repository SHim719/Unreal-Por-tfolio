// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "RPGPortalActor.generated.h"

class USphereComponent;

UCLASS()
class RPGPROJECT_API ARPGPortalActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	ARPGPortalActor();
	
protected:
	virtual void BeginPlay() override;
	virtual void Interact()	override;

		
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere)
	FName ChangeLevelName;
	
	UPROPERTY(EditAnywhere)
	FName PlayerStartTag;
	
};
