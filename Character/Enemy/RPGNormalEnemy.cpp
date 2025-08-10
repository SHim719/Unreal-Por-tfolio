// 


#include "RPGNormalEnemy.h"
#include "Components/WidgetComponent.h"

ARPGNormalEnemy::ARPGNormalEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	HpBarComponent = CreateDefaultSubobject<UWidgetComponent>("HpBar");
	HpBarComponent->SetupAttachment(GetRootComponent());
}

void ARPGNormalEnemy::BeginPlay()
{
	Super::BeginPlay();

	StartBehaviorTree();
}

UUserWidget* ARPGNormalEnemy::GetHpBarWidget() const
{
	return HpBarComponent->GetUserWidgetObject();
}


