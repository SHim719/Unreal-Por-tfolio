// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPGQuestTask.generated.h"

struct FQuestTaskSignalMsg;

UENUM()
enum class ERPGQuestTaskActionType
{
	KillEnemy,
	ObtainItem,
	MeetNPC,
	End,
};


UCLASS(Abstract, Const, DefaultToInstanced, EditInlineNew, CollapseCategories)
class URPGQuestTask : public UObject
{
	GENERATED_BODY()

public:
	virtual void StartTask();
	virtual bool ReceiveNotifyTask(const FQuestTaskSignalMsg& Msg) PURE_VIRTUAL(URPGQuestTask::ReceiveNotifyTask, return false;)
	virtual FText GetCurrentQuestTaskText() PURE_VIRTUAL(virtual FText GetCurrentQuestTaskText(), return FText::GetEmpty(); )
	virtual bool IsComplete() const PURE_VIRTUAL(virtual bool IsComplete() const, return false; )
	
	virtual URPGQuestTask* Clone(UObject* WorldContextObject) PURE_VIRTUAL(virtual URPGQuestTask* Clone(UObject* WorldContextObject), return nullptr; )
protected:
	ERPGQuestTaskActionType TaskActionType = ERPGQuestTaskActionType::End;

	UPROPERTY(EditDefaultsOnly)
	FText TaskDescription;
	
};

