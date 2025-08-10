// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "RPGPlayerCaptureCharacter.generated.h"

class URPGEquipMeshManagerComponent;
class URPGItemFragment_Attachable;
class ARPGEquipmentActor;
struct FOnPlayerChangeEquipmentMsg;

UCLASS()
class RPGPROJECT_API ARPGPlayerCaptureCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARPGPlayerCaptureCharacter();

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;

	UPROPERTY(EditDefaultsOnly)
	FVector SpawnLocation = FVector(0.f, 0.f, -100000.f);

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	TObjectPtr<URPGEquipMeshManagerComponent> EquipMeshManagerComponent;
};
