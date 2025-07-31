// 


#include "RPGShopManager.h"

#include "RPGHelperLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/RPGPlayerCharacter.h"
#include "Components/RPGInventoryComponent.h"
#include "Data/Shop/RPGShopDatabase.h"
#include "Data/Text/RPGTextManager.h"
#include "Kismet/GameplayStatics.h"
#include "Message/RPGGameMessage.h"
#include "UI/Common/RPGConfirmWidget.h"
#include "UI/Common/RPGDecideCountWidget.h"

void URPGShopManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	GetGameInstance()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
	{
		if (ARPGHUD* HUD = RPGHelper::GetHUD_Safe(this))
		{
			if (DecideCountWidget = CreateWidget<URPGDecideCountWidget>(GetGameInstance(), HUD->GetSplitQuantityWidgetClass()))
			{
				DecideCountWidget->SetVisibility(ESlateVisibility::Collapsed);
				DecideCountWidget->AddToViewport();
				DecideCountWidget->OnCancelDelegate.BindUObject(this, &ThisClass::ClearActionState);
			}
			
			if (ConfirmWidget = CreateWidget<URPGConfirmWidget>(GetGameInstance(), HUD->GetConfirmWidgetClass()))
			{
				ConfirmWidget->SetVisibility(ESlateVisibility::Collapsed);
				ConfirmWidget->AddToViewport();
				ConfirmWidget->OnCancelButtonClicked.BindUObject(this, &ThisClass::ClearActionState);
			}
		}
	}));
	
}

void URPGShopManager::OpenShop(const FName& ShopId)
{
	const FRPGShopDataRow* ShopData = RPGHelper::GetShopData_Safe(this, ShopId);
	if (ShopData == nullptr)
		return;

	CurrentShopId = ShopId;
	ClearActionState();
	
}

void URPGShopManager::RequestBuyItem(const int32 ShopEntryIndex) 
{
	const FRPGShopDataRow* ShopData = RPGHelper::GetShopData_Safe(this, CurrentShopId);
	if (ShopData == nullptr)
		return;

	if (ShopEntryIndex >= ShopData->SellItemIds.Num())
		return;

	ShopActionState = EShopActionState::Buy;
	CurrentBuyItemId = ShopData->SellItemIds[ShopEntryIndex];

	if (const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(this, CurrentBuyItemId))
	{
		FVector2D ScreenMousePos;
		if (!RPGHelper::GetMousePosition(this, ScreenMousePos))
			return;
		
		FVector2D PixelPosition, ViewportPosition;
		USlateBlueprintLibrary::AbsoluteToViewport(this, ScreenMousePos, PixelPosition, ViewportPosition);
		CurrentWidgetPos = PixelPosition;
		
		if (ItemDef->StackCount > 1)
		{
			ConfirmItemCount(ItemDef->StackCount);
		}
		else
		{
			OnConfirmCount(1);
		}
	}
	
}

void URPGShopManager::RequestSellItem(const ESlotType& SlotType, const int32 SlotIndex, const FVector2D& ScreenMousePos)
{
	auto* PlayerState = RPGHelper::GetPlayerState_Safe(this);
	if (PlayerState == nullptr)
		return;

	auto* InventoryComponent = PlayerState->FindComponentByClass<URPGInventoryComponent>();
	if (InventoryComponent == nullptr)
		return;

	ShopActionState = EShopActionState::Sell;

	FRPGInventoryEntry InventoryEntry;
	if (InventoryComponent->GetInventoryEntry(static_cast<EItemType>(SlotType), SlotIndex, InventoryEntry) == false)
		return;

	const int32 StackCount = InventoryEntry.GetStackCount();
	const URPGItemInstance* ItemInstance = InventoryEntry.GetItemInstance();
	if (InventoryEntry.GetItemInstance() == nullptr || StackCount == 0)
		return;

	CurrentSellItemInstance = ItemInstance;
	SellItemSlotIndex = SlotIndex;

	FVector2D PixelPosition, ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(this, ScreenMousePos, PixelPosition, ViewportPosition);
	CurrentWidgetPos = PixelPosition;
	
	if (StackCount > 1)
	{
		ConfirmItemCount(StackCount);
	}
	else
	{
		OnConfirmCount(1);
	}
	
}

void URPGShopManager::BuyItem(const int32 Count) 
{
	if (CurrentBuyItemId == NAME_None || Count == 0)
		return;

	FName ItemId = CurrentBuyItemId;
	CurrentBuyItemId = NAME_None;

	auto* PlayerState = RPGHelper::GetPlayerState_Safe(this);
	if (PlayerState == nullptr)
		return;

	auto* InventoryComponent = PlayerState->FindComponentByClass<URPGInventoryComponent>();
	if (InventoryComponent == nullptr)
		return;

	if (InventoryComponent->CanInsertItem(ItemId, Count) == false)
	{
		// 인벤토리 공간 X
		return;
	}

	const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(this, ItemId);
	if (ItemDef == nullptr)
		return;

	const int32 BuyPrice = ItemDef->BuyPrice * Count;

	if (InventoryComponent->HasGold(BuyPrice))
	{
		InventoryComponent->AddGold(-BuyPrice);
		InventoryComponent->InsertItem(ItemId, Count);
	}
	else
	{
		// 돈부족
	}

	ClearActionState();
}



void URPGShopManager::ClearActionState()
{
	CurrentSellItemInstance = nullptr;
	SellItemSlotIndex = -1;
	CurrentActionItemCount = 0;

	CurrentBuyItemId = NAME_None;
}

void URPGShopManager::ConfirmItemCount(const int32 MaxCount)
{
	DecideCountWidget->SetVisibility(ESlateVisibility::Visible);
	DecideCountWidget->Init(MaxCount);
	DecideCountWidget->SetPositionInViewport(CurrentWidgetPos);
	
	DecideCountWidget->OnDecideCountDelegate.BindUObject(this, &ThisClass::OnConfirmCount);
}

void URPGShopManager::OnConfirmCount(const int32 Count)
{
	CurrentActionItemCount = Count;
	if (CurrentActionItemCount <= 0)
	{
		ClearActionState();
		return;
	}

	ConfirmWidget->SetVisibility(ESlateVisibility::Visible);
	ConfirmWidget->SetPositionInViewport(CurrentWidgetPos);
	
	if (ShopActionState == EShopActionState::Buy)
	{
		ConfirmWidget->SetDescriptionText(MakeBuyConfirmText());
		ConfirmWidget->OnConfirmButtonClicked.BindUObject(this, &ThisClass::BuyItem, CurrentActionItemCount);
	}
	else if (ShopActionState == EShopActionState::Sell)
	{
		ConfirmWidget->SetDescriptionText(MakeSellConfirmText());
		ConfirmWidget->OnConfirmButtonClicked.BindUObject(this, &ThisClass::SellItem, CurrentActionItemCount);
	}
}


void URPGShopManager::SellItem(const int32 Count)
{
	if (CurrentSellItemInstance == nullptr || Count == 0)
		return;

	auto* PlayerState = RPGHelper::GetPlayerState_Safe(this);
	if (PlayerState == nullptr)
		return;

	auto* InventoryComponent = PlayerState->FindComponentByClass<URPGInventoryComponent>();
	if (InventoryComponent == nullptr)
		return;
	
	FRPGInventoryEntry InventoryEntry;
	if (InventoryComponent->GetInventoryEntry(CurrentSellItemInstance->GetItemType(), SellItemSlotIndex, InventoryEntry) == false)
		return;

	if (InventoryEntry.GetStackCount() >= Count)
	{
		if (const FRPGItemDefinition* ItemDef = CurrentSellItemInstance->GetItemDef())
		{
			const int32 SellPrice = ItemDef->SellPrice * Count;
			
			InventoryComponent->AddGold(SellPrice);
			InventoryComponent->AddStackCount(CurrentSellItemInstance, -Count);
		}
	}
	else
	{
		// 아이템 개수 부족
	}
	
	ClearActionState();
}

FText URPGShopManager::MakeBuyConfirmText() const
{
	if (auto* TextMgr = RPGHelper::GetTextManager(this))
	{
		if (const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(this, CurrentBuyItemId))
		{
			FText ConfirmText = TextMgr->GetUIText(TEXT("Confirm_BuyItem"));
		
			FFormatNamedArguments Args;
			Args.Add(TEXT("ItemName"), ItemDef->DisplayName);
			Args.Add(TEXT("ItemCount"), CurrentActionItemCount);
			Args.Add(TEXT("TotalGold"), FMath::Clamp(ItemDef->BuyPrice * CurrentActionItemCount, 0, INT32_MAX));

			return FText::Format(ConfirmText, Args);
		}
	}

	return FText::GetEmpty();
}

FText URPGShopManager::MakeSellConfirmText() const
{
	if (auto* TextMgr = RPGHelper::GetTextManager(this))
	{
		if (const FRPGItemDefinition* ItemDef = CurrentSellItemInstance->GetItemDef())
		{
			FText ConfirmText = TextMgr->GetUIText(TEXT("Confirm_SellItem"));
		
			FFormatNamedArguments Args;
			Args.Add(TEXT("ItemName"), ItemDef->DisplayName);
			Args.Add(TEXT("ItemCount"), CurrentActionItemCount);
			Args.Add(TEXT("TotalGold"), FMath::Clamp(ItemDef->SellPrice * CurrentActionItemCount, 0, INT32_MAX));

			return FText::Format(ConfirmText, Args);
		}
	}

	return FText::GetEmpty();
}





