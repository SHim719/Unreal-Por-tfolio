#include "Npc/RPGNpcBase.h"

#include "RPGHelperLibrary.h"
#include "Components/WidgetComponent.h"
#include "Data/Npc/RPGNpcData.h"
#include "Data/Npc/RPGNpcDatabase.h"
#include "Message/RPGGameMessage.h"
#include "UI/Npc/RPGNpcDescTextWidget.h"


ARPGNpcBase::ARPGNpcBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	NpcDescWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("NpcDescWidgetComponent");
	NpcDescWidgetComponent->SetupAttachment(GetRootComponent());

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ARPGNpcBase::BeginPlay()
{
	Super::BeginPlay();

	InitNpcDescWidget();
}

void ARPGNpcBase::Interact()
{
	EndInteractionMsgHandler = RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("NPCInteraction.EndInteraction")),
		this, &ThisClass::HandleEndInteraction);
}

void ARPGNpcBase::HandleEndInteraction(FGameplayTag Tag, const FEndNpcInteractionMsg& Msg)
{
	EndInteractionMsgHandler.Unregister();
}


void ARPGNpcBase::InitNpcDescWidget()
{
	if (NpcDescWidgetComponent == nullptr)
		return;

	const FRPGNpcDataRow* NpcDataRow = RPGHelper::GetNpcData_Safe(this, NpcId);
	if (NpcDataRow == nullptr)
		return;

	if (URPGNpcDescTextWidget* NpcDescTextWidget = Cast<URPGNpcDescTextWidget>(NpcDescWidgetComponent->GetWidget()))
	{
		NpcDescTextWidget->SetNpcName(NpcDataRow->DisplayName);
	}
}






