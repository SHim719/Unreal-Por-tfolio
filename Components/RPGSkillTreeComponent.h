

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Data/Skill/RPGSkillData.h"
#include "RPGSkillTreeComponent.generated.h"


class URPGSkillInstance;
struct FOnClickSkillLevelUpMsg;
struct FOnAttributeChangeData;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillPointChanged, const int32 CurrentSkillPoint);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillLevelChanged, const FGameplayTag& SkillTag, const int32 CurrentSkillLevel);


class URPGAbilitySystemComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API URPGSkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URPGSkillTreeComponent();
	
	void InitializeSkillTreeByData(const URPGSkillTreeData* SkillTreeData);
	FGameplayTag GetSkillTagByIndex(const int32 Index);
	URPGSkillInstance* GetSkillInstance(const int32 Index);
	
	void AddSkillPoint(const int32 Magnitude);
	int32 GetSkillPoint() const { return CurSkillPoint; }

	void GetSkillLevels(TMap<FGameplayTag, int32>& SkillLevels);
	const FRPGSkillTreeNode* FindSkillTreeNodeByTag(const FGameplayTag& SkillTag) const;

	FOnSkillPointChanged			OnSkillPointChanged;
	FOnSkillLevelChanged			OnSkillLevelChanged;
	
private:
	void HandleSkillLevelChanged(const FGameplayTag& SkillTag, const int32 CurSkillLevel) const;
	void HandleSkillPointUpPressed(FGameplayTag Tag, const FOnClickSkillLevelUpMsg& Msg);
	
	// 스킬 레벨을 올리면 그 스킬이 선행 조건인 다른 스킬의 활성화 여부 체크
	void CheckChildSkillActivationCondition(const FGameplayTag& SkillTag) const;
	bool IsMeetSkillActivationCondition(const FRPGSkillTreeNode* Node) const;
	bool CanSkillLearn(const FGameplayTag& SkillTag) const;
	void SetSkillReadyToLearn(const FGameplayTag& SkillTag) const;

private:
	UPROPERTY()
	int32 CurSkillPoint = 0;

private:
	UPROPERTY()
	TObjectPtr<URPGAbilitySystemComponent> OwnerASC;

	UPROPERTY()
	TArray<FRPGSkillTreeNode> SkillTreeNodes;

	UPROPERTY()
	TArray<TObjectPtr<URPGSkillInstance>> SkillInstances;

	// 부모 -> 자식 방향 그래프, 현재 스킬포인트를 올렸을 때, 연결된 스킬 노드의 스킬 활성화 여부를 조사하기 위함.
	TMap<FGameplayTag, TArray<FGameplayTag>> ParentsToChildSkills;

	int32 MaxColumn = 0;
};


