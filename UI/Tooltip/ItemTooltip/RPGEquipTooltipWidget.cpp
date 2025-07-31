#include "RPGEquipTooltipWidget.h"

#include "Components/TextBlock.h"
#include "Data/Item/RPGItemDatabase.h"
#include "Item/RPGItemInstance.h"
#include "UI/Tooltip/TooltipContent/RPGTooltipContent.h"

void URPGEquipTooltipWidget::Init(const URPGTooltipContent* TooltipContent)
{
	Super::Init(TooltipContent);
	
	const URPGEquipmentTooltipContent* EquipmentTooltipContent = Cast<URPGEquipmentTooltipContent>(TooltipContent);
	if (EquipmentTooltipContent == nullptr || EquipmentTooltipContent->ItemInstance == nullptr)
		return;

	TArray<FText> OptionTexts;
	OptionTexts.Add(FText::FromString(TEXT("기본 옵션")));
	AddItemOptionRowText(EquipmentTooltipContent->ItemInstance->GetItemOptions(), OptionTexts);

	Text_OptionTexts->SetText( FText::Join(FText::FromString(TEXT("\n")), OptionTexts));

	OptionTexts.Empty();
	OptionTexts.Add(FText::FromString(TEXT("랜덤 옵션")));
	AddItemOptionRowText(EquipmentTooltipContent->ItemInstance->GetRandomOptions(), OptionTexts);

	Text_RandomOptionTexts->SetText( FText::Join(FText::FromString(TEXT("\n")), OptionTexts));
}



void URPGEquipTooltipWidget::AddItemOptionRowText(const TArray<FRPGItemOptionInstance>& ItemOptionInstances, TArray<FText>& OptionTexts)
{
	const URPGItemDatabase* ItemDB = GetGameInstance()->GetSubsystem<URPGItemDatabase>();
	if (ItemDB == nullptr)
		return;
	
	for (const FRPGItemOptionInstance& ItemOptionInstance : ItemOptionInstances)
	{
		const FRPGItemOptionRow* ItemOptionData = ItemOptionInstance.OptionDataRowHandle.GetRow<FRPGItemOptionRow>(TEXT("Query Option"));
		if (ItemOptionData == nullptr)
			return;
		
		FFormatNamedArguments Arguments;
		
		for (const TPair<FGameplayTag, int32>& Pair : ItemOptionInstance.OptionValues)
		{
			FString KeyString = Pair.Key.GetTagName().ToString();
			FText ValueText = FText::AsNumber(Pair.Value);
			Arguments.Add(KeyString, ValueText);
		}

		const FText FinalText = FText::Format(ItemOptionData->OptionText, Arguments);
		OptionTexts.Add(FinalText);
	}
}
