#include "UI/Skill/RPGSkillTreeWidget.h"
#include "AbilitySystemComponent.h"
#include "RPGHelperLibrary.h"
#include "Components/TextBlock.h"
#include "UI/Slot/RPGSlotWidget.h"
#include "RPGSkillNodeWidget.h"
#include "Components/RPGSkillTreeComponent.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Data/Skill/RPGSkillData.h"
#include "Message/RPGGameMessage.h"
#include "Player/RPGPlayerState.h"


void URPGSkillTreeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	BindDelegates();
	
}

int32 URPGSkillTreeWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 BaseLayer  = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
							  bParentEnabled);

	for (auto& Pair : SkillTreeGraph)
	{
		const FVector2D FromSkillCenterAbsolute = GetSkillNodeWidgetAbsolutePos(Pair.Key);
		const TArray<FGameplayTag>& ToSkillEdges = Pair.Value;
		
		for (const FGameplayTag& ToSkillTag : ToSkillEdges)
		{
			if (const URPGSkillNodeWidget* SkillSlotWidget = GetSkillNodeWidgetByTag(ToSkillTag))
			{
				const FLinearColor LineColor = SkillSlotWidget->IsActive() ? FLinearColor::Yellow : FLinearColor(1.f, 1.f, 1.f, 0.5f);
				const FVector2D ToSkillCenterAbsolute = GetSkillNodeWidgetAbsolutePos(ToSkillTag);
				TArray<FVector2D> LinePoints{
					AllottedGeometry.AbsoluteToLocal(FromSkillCenterAbsolute), AllottedGeometry.AbsoluteToLocal(ToSkillCenterAbsolute)};
			
				FSlateDrawElement::MakeLines(OutDrawElements, BaseLayer - DrawLineLayerOffset, AllottedGeometry.ToPaintGeometry(), LinePoints,
					ESlateDrawEffect::None,
					LineColor, 
					true,
					10.f);
			}
		}
	}
	
	return BaseLayer - DrawLineLayerOffset;
}

void URPGSkillTreeWidget::BindDelegates()
{
	if (ARPGPlayerState* PlayerState = RPGHelper::GetPlayerState_Safe(this))
	{
		if (URPGSkillTreeComponent* SkillTreeComponent = PlayerState->GetComponentByClass<URPGSkillTreeComponent>())
		{
			SkillTreeComponent->OnSkillLevelChanged.AddUObject(this, &ThisClass::HandleSkillLevelChanged);
			SkillTreeComponent->OnSkillPointChanged.AddUObject(this, &ThisClass::HandleSkillPointChanged);
		}
	}

	RPGMessage::RegisterListener(GetWorld(), FGameplayTag::RequestGameplayTag("Widget.SkillTree.Initialize"), this, &ThisClass::HandleInitSkillTreeMsg);
	RPGMessage::RegisterListener(GetWorld(), FGameplayTag::RequestGameplayTag("Widget.SkillTree.UpdateTooltip"), this, &ThisClass::HandleUpdateTooltipMsg);
}

void URPGSkillTreeWidget::MakeSkillTreeGraph(const URPGSkillTreeData* SkillTreeData)
{
	for (const FRPGSkillTreeNode& SkillTreeNode : SkillTreeData->SkillNodes)
	{
		for (auto& Pair : SkillTreeNode.RequiredSkills)
		{
			SkillTreeGraph.FindOrAdd(Pair.Key).Add(SkillTreeNode.SkillTag);
		}
	}
}



void URPGSkillTreeWidget::HandleInitSkillTreeMsg(FGameplayTag Tag, const FInitializeSkillTreeWidgetMsg& Msg)
{
	const int32 MaxColumn = Msg.MaxColumn;
	for (const FRPGSkillTreeNode& SkillTreeNode : Msg.SkillTreeData->SkillNodes)
	{
		CachedSkillTreeNodes.Add(SkillTreeNode.SkillTag, SkillTreeNode);
		
		auto SlotWidget = CreateWidget<URPGSkillNodeWidget>(this, SkillSlotWidgetClass);
		Grid_SkillTree->AddChildToUniformGrid(SlotWidget, SkillTreeNode.Row, SkillTreeNode.Col);
		SlotWidget->InitializeSkillSlot(SkillTreeNode, MaxColumn);
	}
	MakeSkillTreeGraph(Msg.SkillTreeData);
	
}

void URPGSkillTreeWidget::HandleUpdateTooltipMsg(FGameplayTag, const FUpdateSkillTooltipMsg& Msg)
{
	if (Msg.SkillTag.IsValid() && Msg.TooltipContent != nullptr)
	{
		if (auto* SkillNodeWidget = GetSkillNodeWidgetByTag(Msg.SkillTag))
		{
			SkillNodeWidget->UpdateTooltip(Msg.TooltipContent);
		}
	}
}


void URPGSkillTreeWidget::HandleSkillPointChanged(const int32 NewSkillPoint) const
{
	Text_SkillPoint->SetText(FText::AsNumber(NewSkillPoint));

	const TArray<UWidget*>& ChildWidgets = Grid_SkillTree->GetAllChildren();
	for (const auto ChildWidget : ChildWidgets)
	{
		if (const URPGSkillNodeWidget* SkillSlotWidget = Cast<URPGSkillNodeWidget>(ChildWidget))
		{
			SkillSlotWidget->HandleSkillPointChanged(NewSkillPoint);
		}
	}
	
}

void URPGSkillTreeWidget::HandleSkillLevelChanged(const FGameplayTag& SkillTag, const int32 NewSkillLevel)
{
	if (const FRPGSkillTreeNode* SkillNode = CachedSkillTreeNodes.Find(SkillTag))
	{
		if (URPGSkillNodeWidget* SkillSlotWidget = GetSkillNodeWidgetByTag(SkillTag))
		{
			const bool IsMastered = SkillNode->MasterLevel == NewSkillLevel;
			SkillSlotWidget->HandleSkillLevelChanged(NewSkillLevel, IsMastered);
		}
	}
}


URPGSkillNodeWidget* URPGSkillTreeWidget::GetSkillNodeWidgetByTag(const FGameplayTag& SkillTag) const
{
	if (const FRPGSkillTreeNode* Node = CachedSkillTreeNodes.Find(SkillTag))
	{
		const TArray<UPanelSlot*>& AllSlots = Grid_SkillTree->GetSlots();
		for (const auto ChildWidget : AllSlots)
		{
			if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(ChildWidget))
			{
				if (GridSlot->GetColumn() == Node->Col && GridSlot->GetRow() == Node->Row)
					return Cast<URPGSkillNodeWidget>(ChildWidget->Content);
			}
		}
	}
	
	
	return nullptr;
}


FVector2D URPGSkillTreeWidget::GetSkillNodeWidgetAbsolutePos(const FGameplayTag& Tag) const
{
	FVector2D SkillSlotCenterAbsolute = FVector2D::ZeroVector;
	if (const FRPGSkillTreeNode* SkillTreeNode = CachedSkillTreeNodes.Find(Tag))
	{
		if (URPGSkillNodeWidget* FindSlotWidget = GetSkillNodeWidgetByTag(Tag))
		{
			SkillSlotCenterAbsolute = FindSlotWidget->GetCachedGeometry().LocalToAbsolute(FindSlotWidget->GetCachedGeometry().GetLocalSize() * 0.5f);
			return SkillSlotCenterAbsolute;
		}
	}

	return SkillSlotCenterAbsolute;
}

