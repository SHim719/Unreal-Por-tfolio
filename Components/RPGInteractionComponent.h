// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGInteractionComponent.generated.h"


class IInteractableInterface;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPGPROJECT_API URPGInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URPGInteractionComponent();

	void Interact();
	void UpdateInteractableObject(const TScriptInterface<IInteractableInterface>& InInteractableObject);
private:
	UPROPERTY()
	TScriptInterface<IInteractableInterface> InteractableObject;

};
