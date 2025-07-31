#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPGDialogueAction.generated.h"


UCLASS(Abstract, Const, DefaultToInstanced, EditInlineNew, CollapseCategories)
class RPGPROJECT_API URPGDialogueAction : public UObject
{
	GENERATED_BODY()

public:
	virtual void Execute(UObject* WorldContextObject) PURE_VIRTUAL(virtual void Execute(), return;)
};


UCLASS()
class RPGPROJECT_API URPGDialogueAction_ShowQuestInfo : public URPGDialogueAction
{
	GENERATED_BODY()

public:
	virtual void Execute(UObject* WorldContextObject) override;

private:
	UPROPERTY(EditDefaultsOnly)
	FName QuestId;	
};