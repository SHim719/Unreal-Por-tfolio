#include "RPGShopWidget.h"
#include "RPGHelperLibrary.h"
#include "RPGShopEntryWidget.h"
#include "Character/RPGPlayerCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Data/Shop/RPGShopDatabase.h"
#include "Message/RPGGameMessage.h"
#include "UI/Slot/RPGDragDropOperation.h"
#include "Shop/RPGShopManager.h"

void URPGShopWidget::Init(const FName& ShopId)
{
	const FRPGShopDataRow* ShopData = RPGHelper::GetShopData_Safe(this, ShopId);
	if (ShopData == nullptr || ShopEntryClass == nullptr || VerticalBox_ShopEntries == nullptr)
		return;

	ShopEntries.Empty();
	VerticalBox_ShopEntries->ClearChildren();

	for (int32 i = 0; i < ShopData->SellItemIds.Num(); ++i)
	{
		if (URPGShopEntryWidget* ShopEntryWidget = CreateWidget<URPGShopEntryWidget>(this, ShopEntryClass))
		{
			ShopEntryWidget->Init(ShopData->SellItemIds[i], i);
			ShopEntryWidget->OnFocusEntryDelegate.BindUObject(this, &ThisClass::OnFocusEntry);
			ShopEntries.Add(ShopEntryWidget);
			VerticalBox_ShopEntries->AddChild(ShopEntryWidget);
		}
	}

	if (auto* ShopManager = RPGHelper::GetShopManager(this))
	{
		ShopManager->OpenShop(ShopId);
	}
	
	CurrentFocusEntryIndex = -1;
	
}

void URPGShopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_BuyItem)
	{
		Button_BuyItem->OnClicked.AddDynamic(this, &ThisClass::OnClicked_BuyButton);
	}
	
}

bool URPGShopWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                  UDragDropOperation* InOperation)
{
	URPGDragDropOperation* Operation = Cast<URPGDragDropOperation>(InOperation);
	if (Operation == nullptr)
		return true;

	if (Operation->FromSlotType >= ESlotType::Inventory_Equip && Operation->FromSlotType <= ESlotType::Inventory_Etc)
	{
		if (auto* ShopManager = RPGHelper::GetShopManager(this))
		{
			ShopManager->RequestSellItem(Operation->FromSlotType, Operation->SlotIndex, InDragDropEvent.GetScreenSpacePosition());
		}
	}
		
	
	return true;
}

void URPGShopWidget::OnClicked_BuyButton()
{
	if (CurrentFocusEntryIndex < 0)
		return;

	if (auto* ShopManager = RPGHelper::GetShopManager(this))
	{
		ShopManager->RequestBuyItem(CurrentFocusEntryIndex);
	}
}

void URPGShopWidget::OnFocusEntry(const int32 Index)
{
	if (CurrentFocusEntryIndex == Index || CurrentFocusEntryIndex >= ShopEntries.Num())
		return;

	if (CurrentFocusEntryIndex >= 0)
		ShopEntries[CurrentFocusEntryIndex]->ReleaseFocus();
	
	CurrentFocusEntryIndex = Index;
}



