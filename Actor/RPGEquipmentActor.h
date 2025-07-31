

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGEquipmentActor.generated.h"

UCLASS()
class RPGPROJECT_API ARPGEquipmentActor : public AActor
{
	GENERATED_BODY()

public:
	ARPGEquipmentActor();
	
	void SetMesh(USkeletalMesh* InMesh);
	
private:
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> Mesh;

};
