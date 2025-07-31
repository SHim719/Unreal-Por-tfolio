// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "RPGPlayerCaptureCharacter.generated.h"

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
	virtual void BeginDestroy() override;
	

private:
	void HandleOnPlayerEquipmentChange(FGameplayTag, const FOnPlayerChangeEquipmentMsg& Msg);

private:
	FGameplayMessageListenerHandle ChangeEquipmentListener;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;

	UPROPERTY()
	TObjectPtr<ARPGEquipmentActor> WeaponActor;

	UPROPERTY(EditDefaultsOnly)
	FVector SpawnLocation = FVector(0.f, 0.f, -100000.f);
	
};
