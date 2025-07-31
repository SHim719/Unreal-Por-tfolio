

#pragma once

#include "RPGDefines.h"
#include "Item/Fragment/RPGItemFragment.h"
#include "RPGItemData.generated.h"


USTRUCT()
struct FRPGItemDefinition
{
	GENERATED_BODY()

public:
	template <typename FragmentClass>
	const FragmentClass* FindFragmentByClass() const;
	
public:
	UPROPERTY(EditDefaultsOnly)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly)
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> IconTexture;

	UPROPERTY(EditDefaultsOnly)
	EItemType ItemType;
	
	UPROPERTY(EditDefaultsOnly)
	int32 StackCount = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 BuyPrice = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 SellPrice = 0;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> DropMesh; 
	
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<URPGItemFragment>> Fragments;
};

template <typename FragmentClass>
const FragmentClass* FRPGItemDefinition::FindFragmentByClass() const
{
	for (URPGItemFragment* Fragment : Fragments)
	{
		if (Fragment && Fragment->IsA(FragmentClass::StaticClass()))
		{
			return Cast<FragmentClass>(Fragment);
		}
	}
	return nullptr;
}

UCLASS(Const)
class URPGItemData : public UDataAsset
{
	GENERATED_BODY()

public:
	URPGItemData();
	
public:
//	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
//
//#if WITH_EDITOR
//	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
//#endif
//	
public:
	const FRPGItemDefinition* FindItemDefinitionByName(const FName& ItemName) const;
	

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FRPGItemDefinition> ItemDefinitions;
	
};








