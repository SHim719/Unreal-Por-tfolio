// 

#pragma once

#include "CoreMinimal.h"
#include "RPGDefines.h"
#include "Components/ActorComponent.h"
#include "RPGEquipMeshManagerComponent.generated.h"


struct FOnPlayerChangeEquipmentMsg;
class ARPGEquipmentActor;

USTRUCT(BlueprintType)
struct FSkeletalMeshes
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,	BlueprintReadWrite)
	TArray<USkeletalMeshComponent*> SkeletalMeshses;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPGPROJECT_API URPGEquipMeshManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URPGEquipMeshManagerComponent();

protected:
	virtual void BeginPlay() override;

	void HandleOnPlayerEquipmentChange(FGameplayTag, const FOnPlayerChangeEquipmentMsg& Msg);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EArmorType, FSkeletalMeshes> ModularSKs;

	UPROPERTY(BlueprintReadWrite)
	TMap<EArmorType, TObjectPtr<USkeletalMeshComponent>> ArmorSKs;

	UPROPERTY()
	TObjectPtr<ARPGEquipmentActor> WeaponActor;
	
};
