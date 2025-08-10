// 

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGSequenceManager.generated.h"

class ULevelSequence;
class ULevelSequencePlayer;

DECLARE_MULTICAST_DELEGATE(FOnSequenceFinished)
DECLARE_MULTICAST_DELEGATE(FOnSequenceStarted)



UCLASS()
class RPGPROJECT_API URPGSequenceManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void PlaySequence(ULevelSequence* ToPlaySequence);


	FOnSequenceFinished OnSequenceFinished;
	FOnSequenceStarted  OnSequenceStarted;
private:
	UFUNCTION()
	void SequenceFinished();

	void OnWorldDestroyed(UWorld* World);

private:
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> SequencePlayer;

	UPROPERTY()
	TObjectPtr<AActor> OriginalViewTarget;
};
