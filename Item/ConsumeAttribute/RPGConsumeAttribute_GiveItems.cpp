#include "RPGConsumeAttribute_GiveItems.h"

#include "RPGHelperLibrary.h"
#include "Components/RPGInventoryComponent.h"

bool URPGConsumeAttribute_GiveItems::Consume(AActor* Player)
{
	if (Player == nullptr)
		return false;

	URPGInventoryComponent* Inventory = Player->FindComponentByClass<URPGInventoryComponent>();
	if (Inventory == nullptr)
		return false;

	for (auto& Pair : GiveItems)
	{
		FName ItemId = Pair.Key;
		int32 ItemCount = Pair.Value;

		if (Inventory->CanInsertItem(ItemId, ItemCount) == false)
			return false;
	}

	for (auto& Pair : GiveItems)
	{
		FName ItemId = Pair.Key;
		int32 ItemCount = Pair.Value;

		Inventory->InsertItem(ItemId, ItemCount);
	}

	return true;
}

FText URPGConsumeAttribute_GiveItems::GetTooltipText(const UObject* WorldContextObject)
{
	if (OptionRowHandle.IsNull())
		return FText::GetEmpty();
	
	const FRPGItemOptionRow* ItemOptionData = OptionRowHandle.GetRow<FRPGItemOptionRow>(TEXT("Query ItemOption"));
	if (ItemOptionData == nullptr)
		return FText::GetEmpty();
	
	FFormatNamedArguments Arguments;

	TArray<FText> GiveItemTexts;
	GiveItemTexts.Reserve(GiveItems.Num());
	for (auto& Pair : GiveItems)
	{
		FName ItemId = Pair.Key;

		if (const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(WorldContextObject, ItemId))
		{
			GiveItemTexts.Add(ItemDef->DisplayName);
		}
	}
	
	Arguments.Add("Items", FText::Join(FText::FromString(TEXT(", ")), GiveItemTexts));
	const FText FinalText = FText::Format(ItemOptionData->OptionText, Arguments);
	
	return FinalText;
}
