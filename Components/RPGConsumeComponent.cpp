#include "Components/RPGConsumeComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Item/RPGItemInstance.h"
#include "Item/ConsumeAttribute/RPGConsumeAttribute.h"
#include "Item/Fragment/RPGItemFragment_Consume.h"


URPGConsumeComponent::URPGConsumeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

bool URPGConsumeComponent::Consume(const URPGItemInstance* ItemInstance, const int32 InventorySlotIndex) const
{
	const URPGItemFragment_Consume* ItemFragment_Consume = ItemInstance->FindFragmentByClass<URPGItemFragment_Consume>();
	if (ItemFragment_Consume == nullptr)
		return false;

	for (auto ConsumeAttribute : ItemFragment_Consume->ConsumeAttributes)
	{
		ConsumeAttribute->Consume(GetOwner());
	}


	return true;
}



