// 

#pragma once

#include "CoreMinimal.h"
#include "Data/Skill/RPGSkillData.h"
#include "UI/Tooltip/RPGTooltipWidget.h"
#include "RPGSkillTooltipWidget.generated.h"


UCLASS()
class RPGPROJECT_API URPGSkillTooltipWidget : public URPGTooltipWidget
{
	GENERATED_BODY()

public:
	virtual void Init(const URPGTooltipContent* TooltipContent) override;

private:
	void FillSkillLevel(const int32 CurrentSkillLevel);
	void FillPrerequirements(const FRPGSkillTreeNode* SkillTreeNode, int32 CurrentSkillLevel);
	void FillCooldown(float CurrentCooldown);
	void FillDescription(const UCurveTable* SkillAttributeTable, const FText& SkillDescription, const int32 CurrentSkillLevel);
	
private:
	UPROPERTY(EditDefaultsOnly)
	FText SkillLevelDescText;
	
	UPROPERTY(EditDefaultsOnly)
	FText CooldownDescText;

	UPROPERTY(EditDefaultsOnly)
	FText ReqSkillText;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Name;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_SkillLevel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Prerequirements;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Cooldown;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Description;

	
};
