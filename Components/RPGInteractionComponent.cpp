#include "RPGInteractionComponent.h"
#include "Interface/InteractableInterface.h"


URPGInteractionComponent::URPGInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}

void URPGInteractionComponent::Interact()
{
	if (InteractableObject)
	{
		InteractableObject->Interact();
	}
}

void URPGInteractionComponent::UpdateInteractableObject(
	const TScriptInterface<IInteractableInterface>& InInteractableObject)
{
	if (InteractableObject == InInteractableObject)
	{
		InteractableObject = nullptr;
	}
	else
	{
		InteractableObject = InInteractableObject;
	}
	
}





