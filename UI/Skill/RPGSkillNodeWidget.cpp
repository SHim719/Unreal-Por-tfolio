#include "UI/Skill/RPGSkillNodeWidget.h"

#include "RPGHelperLibrary.h"
#include "Components/Button.h"
#include "Components/RPGSkillTreeComponent.h"
#include "Components/TextBlock.h"
#include "Data/Skill/RPGSkillData.h"
#include "Message/RPGGameMessage.h"
#include "UI/Slot/RPGSlotWidget.h"


void URPGSkillNodeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SlotWidget->SetSlotType(ESlotType::Skill);
	SlotWidget->SetCanDragDrop(false);
	
	if (Button_Plus)
	{
		Button_Plus->OnReleased.AddDynamic(this, &ThisClass::OnClick_SkillPointUpButton);
		SetButtonActiveState(false);
	}
}

void URPGSkillNodeWidget::InitializeSkillSlot(const FRPGSkillTreeNode& SkillTreeNode, const int32 MaxColumn)
{
	const FRPGSkillDataRow* SkillRow = RPGHelper::GetSkillData_Safe(this, SkillTreeNode.SkillTag);
	if (SkillRow == nullptr)
		return;

	SlotWidget->UpdateSlot(SkillRow->MainSkillIcon, 1);
	SlotWidget->UpdateSlotActiveState(false);
	SlotWidget->SetSlotIndex(SkillTreeNode.Row * MaxColumn + SkillTreeNode.Col);
	Text_MaxPoint->SetText(FText::AsNumber(SkillTreeNode.MasterLevel));
	Text_CurPoint->SetText(FText::AsNumber(0));

	CachedSkillTag = SkillTreeNode.SkillTag;
}


void URPGSkillNodeWidget::HandleSkillPointChanged(const int32 CurSkillPoint) const
{
	if (bIsMastered == true || bIsLearnable == false)
		return;
	
	if (CurSkillPoint > 0)
	{
		SetButtonActiveState(true);
	}
	else
	{
		SetButtonActiveState(false);
	}
}


void URPGSkillNodeWidget::HandleSkillLevelChanged(const int32 CurSkillLevel, const bool IsMastered)
{
	Text_CurPoint->SetText(FText::AsNumber(CurSkillLevel));
	if (IsMastered)
	{
		bIsMastered = true;
		SetButtonActiveState(false);
	}

	if (bIsActive == false && CurSkillLevel > 0)
	{
		SlotWidget->UpdateSlotActiveState(true);
		SlotWidget->SetCanDragDrop(true);
		bIsActive = true;
	}

	if (CurSkillLevel == 0) // -1 -> 0으로 바뀌는 순간
	{
		bIsLearnable = true;
	}
}

void URPGSkillNodeWidget::UpdateTooltip(const URPGTooltipContent* TooltipContent) const
{
	if (SlotWidget)
	{
		SlotWidget->UpdateTooltipContent(TooltipContent);
	}
}

void URPGSkillNodeWidget::SetButtonActiveState(const bool bActive) const
{
	Button_Plus->SetIsEnabled(bActive);
	if (bActive)
	{
		Text_Button->SetOpacity(1.f);
	}
	else
	{
		Text_Button->SetOpacity(0.2f);
	}
}

void URPGSkillNodeWidget::OnClick_SkillPointUpButton()
{
	const FOnClickSkillLevelUpMsg Msg(CachedSkillTag);
	RPGMessage::BroadcastMessage(GetWorld(), Msg, FGameplayTag::RequestGameplayTag(FName("Widget.SkillTree.SkillPointUp")));
}
