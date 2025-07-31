// 

#pragma once

#include "CoreMinimal.h"
#include "Components/RPGInventoryComponent.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGShopManager.generated.h"

class URPGConfirmWidget;
class URPGDecideCountWidget;


enum class EShopActionState : uint8
{
	Buy,
	Sell,
	End,
};

UCLASS()
class RPGPROJECT_API URPGShopManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void OpenShop(const FName& ShopId);

	void RequestBuyItem(const int32 ShopEntryIndex);
	void RequestSellItem(const ESlotType& SlotType, const int32 SlotIndex, const FVector2D& ScreenMousePos);
	
private:
	void ClearActionState();
	
	void ConfirmItemCount(const int32 MaxCount);
	void OnConfirmCount(const int32 Count);
	
	void BuyItem(const int32 Count);
	void SellItem(const int32 Count);

	FText MakeBuyConfirmText() const;
	FText MakeSellConfirmText() const;

private:
	UPROPERTY()
	TObjectPtr<URPGDecideCountWidget> DecideCountWidget;

	UPROPERTY()
	TObjectPtr<URPGConfirmWidget> ConfirmWidget;
	
	FName CurrentShopId;

	EShopActionState ShopActionState;
	FVector2D CurrentWidgetPos;
	
	UPROPERTY()
	TObjectPtr<const URPGItemInstance> CurrentSellItemInstance;
	int32 SellItemSlotIndex = 0;

	FName CurrentBuyItemId;
	int32 CurrentActionItemCount = 0;
	
	
};

