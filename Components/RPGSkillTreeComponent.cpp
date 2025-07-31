#include "Components/RPGSkillTreeComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "RPGHelperLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Abilities/GameplayAbility.h"
#include "Data/Skill/RPGSkillData.h"
#include "Message/RPGGameMessage.h"
#include "Skill/RPGSkillInstance.h"


URPGSkillTreeComponent::URPGSkillTreeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void URPGSkillTreeComponent::InitializeSkillTreeByData(const URPGSkillTreeData* SkillTreeData)
{
	if (SkillTreeData == nullptr)
		return;

	OwnerASC = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(RPGHelper::GetPlayerState_Safe(this)));
	
	if (auto* AttributeSet = Cast<URPGAttributeSet>(OwnerASC->GetAttributeSet(URPGAttributeSet::StaticClass())))
	{
		OwnerASC->OnAbilityLevelChanged.AddUObject(this, &URPGSkillTreeComponent::HandleSkillLevelChanged);
	}


	MaxColumn = SkillTreeData->FindMaxColumn();
	FInitializeSkillTreeWidgetMsg Msg;
	Msg.SkillTreeData = SkillTreeData;
	Msg.MaxColumn = MaxColumn;
	RPGMessage::BroadcastMessage(GetWorld(), Msg, FGameplayTag::RequestGameplayTag(FName("Widget.SkillTree.Initialize")));

	SkillTreeNodes = SkillTreeData->SkillNodes;
	
	for (const FRPGSkillTreeNode& Node : SkillTreeNodes)
	{
		if (Node.Row < 0) continue;
		for (auto& Pair : Node.RequiredSkills)
		{
			ParentsToChildSkills.FindOrAdd(Pair.Key).Add(Node.SkillTag);
		}
	}

	SkillInstances.SetNum(SkillTreeNodes.Num());
	for (int32 i = 0; i < SkillInstances.Num(); ++i)
	{
		SkillInstances[i] = NewObject<URPGSkillInstance>(this);
		if (SkillInstances[i])
		{
			SkillInstances[i]->Init(SkillTreeNodes[i].SkillTag, OwnerASC);

			URPGSkillTooltipContent* SkillTooltipContent = NewObject<URPGSkillTooltipContent>();
			SkillTooltipContent->SkillInstance = SkillInstances[i];
			
			FUpdateSkillTooltipMsg TooltipMsg;
			TooltipMsg.SkillTag = SkillTreeNodes[i].SkillTag;
			TooltipMsg.TooltipContent = SkillTooltipContent;

			RPGMessage::BroadcastMessage(GetWorld(), TooltipMsg, FGameplayTag::RequestGameplayTag(FName("Widget.SkillTree.UpdateTooltip")));
		}

		// 선행 스킬이 없으므로 활성화(AbilityLevel을 0으로 만든다.)
		if (SkillTreeNodes[i].RequiredSkills.IsEmpty())
		{
			SetSkillReadyToLearn(SkillTreeNodes[i].SkillTag);
		}
	}


	RPGMessage::RegisterListener(GetWorld(), FGameplayTag::RequestGameplayTag(FName("Widget.SkillTree.SkillPointUp")), this, &ThisClass::HandleSkillPointUpPressed);
}



void URPGSkillTreeComponent::HandleSkillLevelChanged(const FGameplayTag& SkillTag, const int32 CurSkillLevel) const
{
	OnSkillLevelChanged.Broadcast(SkillTag, CurSkillLevel);

	CheckChildSkillActivationCondition(SkillTag);
}

void URPGSkillTreeComponent::HandleSkillPointUpPressed(FGameplayTag Tag, const FOnClickSkillLevelUpMsg& Msg)
{
	const FRPGSkillTreeNode* SkillNode = FindSkillTreeNodeByTag(Msg.SkillTag);
	if (SkillNode == nullptr)
		return;
	
	if (IsMeetSkillActivationCondition(SkillNode))
	{
		auto* ASC = RPGHelper::GetPlayerAbilitySystemComponent_Safe(this);
		if (ASC == nullptr)
			return;

		ASC->AddAbilityLevel(Msg.SkillTag, 1);
		
		AddSkillPoint(-1);
	}
}

const FRPGSkillTreeNode* URPGSkillTreeComponent::FindSkillTreeNodeByTag(const FGameplayTag& SkillTag) const
{
	for (const auto& Node : SkillTreeNodes)
	{
		if (SkillTag == Node.SkillTag)
			return &Node;
	}
	return nullptr;	
}


void URPGSkillTreeComponent::CheckChildSkillActivationCondition(const FGameplayTag& SkillTag) const
{
	if (const TArray<FGameplayTag>* ChildSkills = ParentsToChildSkills.Find(SkillTag))
	{
		for (const FGameplayTag& ChildSkillTag : *ChildSkills)
		{
			if (CanSkillLearn(ChildSkillTag) == false)
			{
				if (const FRPGSkillTreeNode* Node = FindSkillTreeNodeByTag(ChildSkillTag))
				{
					if (IsMeetSkillActivationCondition(Node))
					{
						SetSkillReadyToLearn(ChildSkillTag);
					}
				}
			}
		}
	}
}

bool URPGSkillTreeComponent::IsMeetSkillActivationCondition(const FRPGSkillTreeNode* Node) const
{
	for (auto& Pair : Node->RequiredSkills)
	{
		if (OwnerASC->GetAbilityLevel(Pair.Key) < Pair.Value)
			return false;
	}

	return true;
}

bool URPGSkillTreeComponent::CanSkillLearn(const FGameplayTag& SkillTag) const
{
	return OwnerASC->GetAbilityLevel(SkillTag) > -1;
}

void URPGSkillTreeComponent::SetSkillReadyToLearn(const FGameplayTag& SkillTag) const
{
	OwnerASC->SetAbilityLevel(SkillTag, 0);
}

FGameplayTag URPGSkillTreeComponent::GetSkillTagByIndex(const int32 Index)
{
	const int32 Col = Index % MaxColumn;
	const int32 Row = Index / MaxColumn;
	for (const auto& SkillNode : SkillTreeNodes)
	{
		if (SkillNode.Col == Col && SkillNode.Row == Row)
			return SkillNode.SkillTag;
	}

	return FGameplayTag::EmptyTag;
}

URPGSkillInstance* URPGSkillTreeComponent::GetSkillInstance(const int32 Index)
{
	const int32 Col = Index % MaxColumn;
	const int32 Row = Index / MaxColumn;
	for (int32 i = 0; i < SkillTreeNodes.Num(); ++i)
	{
		if (SkillTreeNodes[i].Col == Col && SkillTreeNodes[i].Row == Row)
			return SkillInstances[i];
	}
	return nullptr;
}

void URPGSkillTreeComponent::AddSkillPoint(const int32 Magnitude)
{
	CurSkillPoint = FMath::Clamp(CurSkillPoint + Magnitude, 0, INT32_MAX - 1);

	OnSkillPointChanged.Broadcast(CurSkillPoint);
}

void URPGSkillTreeComponent::GetSkillLevels(TMap<FGameplayTag, int32>& SkillLevels)
{
	if (OwnerASC)
	{
		for (const auto& SkillTreeNode : SkillTreeNodes)
		{
			SkillLevels.Add(SkillTreeNode.SkillTag, OwnerASC->GetAbilityLevel(SkillTreeNode.SkillTag));
		}
	}
}







