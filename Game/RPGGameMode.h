#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RPGGameMode.generated.h"


class ARPGTargetCircle;
class ARPGDropItemActor;

UCLASS()
class RPGPROJECT_API ARPGGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARPGDropItemActor> DropItemActorClass;


};
