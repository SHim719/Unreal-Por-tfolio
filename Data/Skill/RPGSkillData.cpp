#include "RPGSkillData.h"

void URPGSkillDatabase::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	SkillTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Skill/DT_SkillTable"));
	check(SkillTable);
}

void URPGSkillDatabase::Deinitialize()
{
	Super::Deinitialize();
}

const FRPGSkillDataRow* URPGSkillDatabase::GetSkillDataByName(const FName& Name) const
{
	return SkillTable->FindRow<FRPGSkillDataRow>(Name, TEXT("Find_SkillRow"));
}

const FRPGSkillDataRow* URPGSkillDatabase::GetSkillDataByTag(const FGameplayTag& Tag) const
{
	return SkillTable->FindRow<FRPGSkillDataRow>(Tag.GetTagName(), TEXT("Find_SkillRow"));
	
}

int32 URPGSkillTreeData::FindMaxColumn() const
{ 
	int32 MaxColumn = 0;
	for (const auto& SkillNode : SkillNodes)
	{
		MaxColumn = FMath::Max(MaxColumn, SkillNode.Col);
	}
	return MaxColumn + 1;
}
