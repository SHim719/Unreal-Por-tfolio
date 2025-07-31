

#pragma once

#include "CoreMinimal.h"
#include "RPGDefines.h"
#include "Data/Item/RPGItemData.h"
#include "Data/Item/RPGItemDatabase.h"
#include "Data/Item/RPGItemOption.h"
#include "RPGItemInstance.generated.h"


class URPGTooltipContent;
struct FRPGItemDefinition;

UCLASS()
class RPGPROJECT_API URPGItemInstance : public UObject
{
	GENERATED_BODY()

public:
	URPGItemInstance();
	
public:
	void Init(const FName& InItemId, bool bCreateNew = true);
	void InitTooltipContent();
	bool IsSameItem(const FName& InItemId) const { return ItemId == InItemId;}
	int32 GetMaxStackCount() const;
	EItemType GetItemType() const;
	FName GetItemId() const;
	const URPGTooltipContent* GetTooltipContent() const { return TooltipContent;}
	
	const TArray<FRPGItemOptionInstance>& GetItemOptions() const { return ItemOptions;}
	const TArray<FRPGItemOptionInstance>& GetRandomOptions() const { return ItemRandomOptions;}

	void SetItemOptions(const TArray<FRPGItemOptionInstance>& InItemOptions) { ItemOptions = InItemOptions;}
	void SetRandomOptions(const TArray<FRPGItemOptionInstance>& InRandomItemOptions) { ItemRandomOptions = InRandomItemOptions;}

	const FRPGItemDefinition* GetItemDef() const;
	
	template <typename FragmentClass>
	const FragmentClass* FindFragmentByClass() const;

private:
	void InitItemOptions();
	void GenerateRandomItemOptions();
	
private:
	FName 	ItemId = NAME_None;
	TArray<FRPGItemOptionInstance> ItemOptions;
	TArray<FRPGItemOptionInstance> ItemRandomOptions;

	UPROPERTY()
	TObjectPtr<const URPGTooltipContent> TooltipContent;
};

template <typename FragmentClass>
const FragmentClass* URPGItemInstance::FindFragmentByClass() const
{
	if (const UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		if (const URPGItemDatabase* ItemDB = GameInstance->GetSubsystem<URPGItemDatabase>())
		{
			if (const FRPGItemDefinition* ItemDefinition = ItemDB->GetItemDataByName(ItemId))
			{
				return ItemDefinition->FindFragmentByClass<FragmentClass>();
			}
		}
	}
	return nullptr;
}
