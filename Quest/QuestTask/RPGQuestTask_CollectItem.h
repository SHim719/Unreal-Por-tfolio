// 

#pragma once

#include "CoreMinimal.h"
#include "RPGQuestTask.h"
#include "RPGQuestTask_CollectItem.generated.h"


struct FQuestTaskSignalMsg;

UCLASS()
class URPGQuestTask_CollectItem : public URPGQuestTask
{
	GENERATED_BODY()

public:
	URPGQuestTask_CollectItem()
	{
		TaskActionType = ERPGQuestTaskActionType::ObtainItem;
	}

	virtual void StartTask() override;
	virtual bool ReceiveNotifyTask(const FQuestTaskSignalMsg& Msg) override;
	virtual bool IsComplete() const override { return CurCollectCount >= TargetCollectCount;}
	virtual URPGQuestTask* Clone(UObject* WorldContextObject) override;
	virtual FText GetCurrentQuestTaskText() override;
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> TargetItemIds;

	UPROPERTY(EditDefaultsOnly)
	int32 TargetCollectCount = 0;

	int32 CurCollectCount = 0;
};