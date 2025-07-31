// 

#pragma once

#include "CoreMinimal.h"
#include "RPGQuestTask.h"
#include "RPGQuestTask_KillEnemy.generated.h"


UCLASS()
class URPGQuestTask_KillEnemy : public URPGQuestTask
{
	GENERATED_BODY()
public:
	URPGQuestTask_KillEnemy()
	{
		TaskActionType = ERPGQuestTaskActionType::KillEnemy;
	}

	virtual bool IsComplete() const override { return HuntCount == CurHuntCount;}
	virtual bool ReceiveNotifyTask(const FQuestTaskSignalMsg& Msg) override;
	virtual URPGQuestTask* Clone(UObject* WorldContextObject) override;
	virtual FText GetCurrentQuestTaskText() override;
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> TargetEnemyIds;

	UPROPERTY(EditDefaultsOnly)
	int32 HuntCount = 0;

	int32 CurHuntCount = 0;
};
