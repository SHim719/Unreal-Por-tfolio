#include "Item/RPGItemInstance.h"

#include "RPGHelperLibrary.h"
#include "Algo/RandomShuffle.h"
#include "Fragment/RPGItemFragment_Equippable.h"
#include "UI/Tooltip/TooltipContent/RPGTooltipContent.h"

URPGItemInstance::URPGItemInstance()
{
}

void URPGItemInstance::Init(const FName& InItemId, bool bCreateNew)
{
	ItemId = InItemId;

	InitTooltipContent();

	if (bCreateNew == true)
	{
		InitItemOptions();
		GenerateRandomItemOptions();
	}
}

int32 URPGItemInstance::GetMaxStackCount() const
{
	if (const FRPGItemDefinition* ItemDefinition = GetItemDef())
	{
		return ItemDefinition->StackCount;
	}

	return 0;
}

EItemType URPGItemInstance::GetItemType() const
{
	if (const FRPGItemDefinition* ItemDefinition = RPGHelper::GetItemDefinition_Safe(this, ItemId))
	{
		return ItemDefinition->ItemType;
	}
	
	return EItemType::End;
}

FName URPGItemInstance::GetItemId() const
{
	return ItemId;
}

void URPGItemInstance::InitTooltipContent()
{
	if (GetItemType() == EItemType::Equipment)
	{
		if (URPGEquipmentTooltipContent* EquipTooltipContent = NewObject<URPGEquipmentTooltipContent>(this))
		{
			EquipTooltipContent->ItemInstance = this;
			EquipTooltipContent->ItemId = ItemId;
			TooltipContent = EquipTooltipContent;
		}
	}
	else
	{
		TooltipContent = RPGHelper::GetItemTooltipContent(this, ItemId);
	}
	
}

const FRPGItemDefinition* URPGItemInstance::GetItemDef() const
{
	return RPGHelper::GetItemDefinition_Safe(this, ItemId);
}

void URPGItemInstance::InitItemOptions()
{
	auto* ItemFragment_Equippable = FindFragmentByClass<URPGItemFragment_Equippable>();
	if (ItemFragment_Equippable == nullptr ||  ItemFragment_Equippable->MainOptionInfos.IsEmpty() == true)
		return;

	ItemOptions.Reserve(ItemFragment_Equippable->MainOptionInfos.Num());
	for (auto& ItemOptionInfo : ItemFragment_Equippable->MainOptionInfos)
	{
		if (ItemOptionInfo.OptionDataRowHandle.IsNull() == true)
			continue;
		
		const FRPGItemOptionRow* ItemOptionData = ItemOptionInfo.OptionDataRowHandle.GetRow<FRPGItemOptionRow>("Query ItemOption");
		if (ItemOptionData == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("아이템 옵션을 찾을 수 없습니다."));
			return;
		}

		FRPGItemOptionInstance ItemOptionInstance;
		ItemOptionInstance.OptionDataRowHandle = ItemOptionInfo.OptionDataRowHandle;
		ItemOptionInstance.OptionValues.Reserve(ItemOptionInfo.OptionValueRanges.Num());
		for (auto& ItemOptionValueRange : ItemOptionInfo.OptionValueRanges)
		{
			TPair<FGameplayTag, float> OptionValue = { ItemOptionValueRange.ValueTag, FMath::RandRange(ItemOptionValueRange.MinValue, ItemOptionValueRange.MaxValue) };
			ItemOptionInstance.OptionValues.Add(OptionValue);
		}
		
		ItemOptions.Add(ItemOptionInstance);
	}
}

void URPGItemInstance::GenerateRandomItemOptions()
{
	auto* ItemFragment_Equippable = FindFragmentByClass<URPGItemFragment_Equippable>();
	if (ItemFragment_Equippable == nullptr || ItemFragment_Equippable->RandomOptionCount == 0 || ItemFragment_Equippable->RandomOptionInfos.IsEmpty() == true)
		return;
	
	// 랜덤 옵션을 고르기 위해 인덱스를 배열에 저장하고 섞는다.
	TArray<int32> OptionIndices;
	OptionIndices.Reserve(ItemFragment_Equippable->RandomOptionInfos.Num());
	for (int32 i = 0; i < ItemFragment_Equippable->RandomOptionInfos.Num(); ++i)
	{
		OptionIndices.Add(i);
	}
	Algo::RandomShuffle(OptionIndices);

	
	ItemRandomOptions.Reserve(ItemFragment_Equippable->RandomOptionCount);
	for (int32 i = 0; i < ItemFragment_Equippable->RandomOptionCount; ++i)
	{
		const int32 OptionIndex = OptionIndices[i];
		const FRPGItemOptionRandomValueInfo& OptionRandomValueInfo = ItemFragment_Equippable->RandomOptionInfos[OptionIndex];

		if (OptionRandomValueInfo.OptionDataRowHandle.IsNull() == true)
			continue;
		
		const FRPGItemOptionRow* ItemOptionData = OptionRandomValueInfo.OptionDataRowHandle.GetRow<FRPGItemOptionRow>("Query ItemOption");
		if (ItemOptionData == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("아이템 옵션을 찾을 수 없습니다."));
			return;
		}

		FRPGItemOptionInstance ItemOptionInstance;
		ItemOptionInstance.OptionDataRowHandle = OptionRandomValueInfo.OptionDataRowHandle;
		ItemOptionInstance.OptionValues.Reserve(OptionRandomValueInfo.OptionValueRanges.Num());
		for (auto& ItemOptionValueRange : OptionRandomValueInfo.OptionValueRanges)
		{
			TPair<FGameplayTag, float> OptionValue = { ItemOptionValueRange.ValueTag, FMath::RandRange(ItemOptionValueRange.MinValue, ItemOptionValueRange.MaxValue) };
			ItemOptionInstance.OptionValues.Add(OptionValue);
		}
		
		ItemRandomOptions.Add(ItemOptionInstance);
	}
	
}
