// 

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGQuestDatabase.generated.h"


struct FRPGQuestDefinition;
class URPGQuestData;

UCLASS()
class RPGPROJECT_API URPGQuestDatabase : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const FRPGQuestDefinition* GetQuestDefinitionByName(const FName& QuestId) const;
	
	
private:	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URPGQuestData> QuestData;
};
