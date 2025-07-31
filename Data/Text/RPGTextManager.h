// 

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGTextManager.generated.h"


UCLASS()
class RPGPROJECT_API URPGTextManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	FText GetUIText(const FString& Key) const;

private:
	UPROPERTY()
	TObjectPtr<UStringTable> UICommonTextTable;
};
