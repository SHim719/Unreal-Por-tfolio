#include "RPGSkillTooltipWidget.h"

#include "RPGHelperLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Skill/RPGSkillData.h"
#include "Skill/RPGSkillInstance.h"
#include "UI/Tooltip/TooltipContent/RPGTooltipContent.h"

void URPGSkillTooltipWidget::Init(const URPGTooltipContent* TooltipContent)
{
	Text_SkillLevel->SetVisibility(ESlateVisibility::Collapsed);
	Text_Cooldown->SetVisibility(ESlateVisibility::Collapsed);
	Text_Prerequirements->SetVisibility(ESlateVisibility::Collapsed);
	Text_Description->SetVisibility(ESlateVisibility::Collapsed);
	

	const URPGSkillTooltipContent* SkillTooltipContent = Cast<URPGSkillTooltipContent>(TooltipContent);
	if (SkillTooltipContent == nullptr)
		return;

	const URPGSkillInstance* SkillInstance = SkillTooltipContent->SkillInstance;
	if (SkillInstance == nullptr)
		return;

	if (const FRPGSkillDataRow* SkillData = RPGHelper::GetSkillData_Safe(this, SkillTooltipContent->SkillInstance->GetSkillTag()))
	{
		const int32 CurrentSkillLevel = SkillInstance->GetCurrentSkillLevel();
		
		if (Text_Name)
		{
			Text_Name->SetText(SkillData->SkillName);
		}

		if (Image_Icon)
		{
			Image_Icon->SetBrushFromTexture(SkillData->MainSkillIcon);
		}
		
		FillSkillLevel(CurrentSkillLevel);
		FillPrerequirements(SkillInstance->GetSkillTreeNode(), CurrentSkillLevel);

		if (CurrentSkillLevel > 0)
		{
			FillCooldown(SkillInstance->GetCurrentCooldown());
			FillDescription(SkillData->SkillAttributeTable, SkillData->SkillDescription, CurrentSkillLevel);
		}
	}
}

void URPGSkillTooltipWidget::FillSkillLevel(const int32 CurrentSkillLevel)
{
	if (Text_SkillLevel && CurrentSkillLevel >= 0)
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("SkillLevel"), FText::AsNumber(CurrentSkillLevel));
			
		Text_SkillLevel->SetText(FText::Format(SkillLevelDescText, Arguments));
		Text_SkillLevel->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void URPGSkillTooltipWidget::FillPrerequirements(const FRPGSkillTreeNode* SkillTreeNode, const int32 CurrentSkillLevel)
{
	if (Text_Prerequirements && CurrentSkillLevel == -1)
	{
		TArray<FText> ReqSkillTexts;
		
		if (SkillTreeNode->RequiredSkills.Num())
		{
			for (auto& Pair : SkillTreeNode->RequiredSkills)
			{
				FFormatNamedArguments Arguments;
				
				FGameplayTag ReqSkillTag = Pair.Key;
				int32 ReqSkillLevel = Pair.Value;
				if (const FRPGSkillDataRow* SkillData = RPGHelper::GetSkillData_Safe(this,  ReqSkillTag))
				{
					Arguments.Add(TEXT("ReqSkill"), SkillData->SkillName);
					Arguments.Add(TEXT("ReqSkillLevel"), ReqSkillLevel);
				}
				
				ReqSkillTexts.Add(FText::Format(ReqSkillText, Arguments));
			}
		}

		Text_Prerequirements->SetVisibility(ESlateVisibility::HitTestInvisible);
		Text_Prerequirements->SetText(FText::Join(FText::FromString(TEXT("\n")), ReqSkillTexts));
	}
}

void URPGSkillTooltipWidget::FillCooldown(const float CurrentCooldown)
{
	if (Text_Cooldown && CurrentCooldown > 0.f)
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("Cooldown"), FText::AsNumber(CurrentCooldown));
			
		Text_Cooldown->SetText(FText::Format(CooldownDescText, Arguments));
		Text_Cooldown->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void URPGSkillTooltipWidget::FillDescription(const UCurveTable* SkillAttributeTable, const FText& SkillDescription, const int32 CurrentSkillLevel)
{
	if (Text_Description && SkillAttributeTable)
	{
		FFormatNamedArguments Arguments;
		for (auto& CurvePair : SkillAttributeTable->GetRowMap())
		{
			if (const FRealCurve* SkillAttributeCurve = CurvePair.Value)
			{
				const float Value = FMath::Abs(SkillAttributeCurve->Eval(CurrentSkillLevel));
				FText AttributeText = FText::AsNumber(Value);
				Arguments.Add(CurvePair.Key.ToString(), AttributeText);
			}
		}
		Text_Description->SetText(FText::Format(SkillDescription, Arguments));
		Text_Description->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}
