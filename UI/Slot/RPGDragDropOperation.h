

#pragma once

#include "CoreMinimal.h"
#include "RPGDefines.h"
#include "Blueprint/DragDropOperation.h"
#include "RPGDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	ESlotType FromSlotType = ESlotType::End;
	
	int32 SlotIndex = 0;
	
};
