// 

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGLevelChangeManager.generated.h"

class URPGFadeScreen;

UCLASS(Blueprintable)
class RPGPROJECT_API URPGLevelChangeManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	
	void ReserveLevelChange(FName DestPlayerStartTag, FName DestLevel);
	void ConfirmLevelChange();
	
	FName PlayerStartTag = FName("Start");
	FName ToChangeLevel;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPGFadeScreen> FadeScreenClass;
	
};
