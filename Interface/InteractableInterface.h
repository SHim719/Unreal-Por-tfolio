// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

UINTERFACE()
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};


class RPGPROJECT_API IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual void Interact();
	
};
