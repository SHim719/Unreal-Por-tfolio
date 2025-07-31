#include "RPGDropItemActor.h"

#include "RPGHelperLibrary.h"
#include "Components/WidgetComponent.h"
#include "Data/Item/RPGItemData.h"
#include "UI/Common/RPGDropItemLabelWidget.h"


ARPGDropItemActor::ARPGDropItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMeshComponent);
	
	ItemLabelWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemLabelWidgetComp"));
	ItemLabelWidgetComponent->SetupAttachment(ItemMeshComponent);
	
}

void ARPGDropItemActor::OnExitPool()
{
	Super::OnExitPool();

	ItemMeshComponent->SetSimulatePhysics(true);
	ItemMeshComponent->SetEnableGravity(true);
}

void ARPGDropItemActor::OnReturnToPool()
{
	Super::OnReturnToPool();

	ItemMeshComponent->SetSimulatePhysics(false);
	ItemMeshComponent->SetEnableGravity(false);
}	


void ARPGDropItemActor::Init(const FName& InItemId, const int32 InItemCount)
{
	if (const FRPGItemDefinition* ItemDef = RPGHelper::GetItemDefinition_Safe(this, InItemId))
	{
		ItemId = InItemId;

		ItemMeshComponent->SetStaticMesh(ItemDef->DropMesh);

		if (URPGDropItemLabelWidget* DropItemLabelWidget = Cast<URPGDropItemLabelWidget>(ItemLabelWidgetComponent->GetWidget()))
		{
			DropItemLabelWidget->SetLabelText(ItemDef->DisplayName, InItemCount);
			DropItemLabelWidget->OnClickItemLabel.BindUObject(this, &ThisClass::RequestInsertItem);
		}

		ItemCount = InItemCount; 
	}
}

void ARPGDropItemActor::RequestInsertItem()
{
	if (auto* PlayerState = RPGHelper::GetPlayerState_Safe(this))
	{
		if (auto* Inventory = PlayerState->FindComponentByClass<URPGInventoryComponent>())
		{
			int32 RemainCount = 0;
			if (Inventory->CanInsertItem(ItemId, ItemCount))
			{
				Inventory->InsertItem(ItemId, ItemCount);
				ReturnToPool();
			}
		}
	}
}




