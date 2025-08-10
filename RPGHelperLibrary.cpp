#include "RPGHelperLibrary.h"

#include "AIController.h"
#include "RPGDefines.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/RPGPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Data/Enemy/RPGEnemyDatabase.h"
#include "Data/Item/RPGItemData.h"
#include "Data/Quest/RPGQuestDatabase.h"
#include "Quest/RPGQuestManager.h"
#include "Data/Item/RPGItemDatabase.h"
#include "Data/Npc/RPGNpcDatabase.h"
#include "Data/Shop/RPGShopDatabase.h"
#include "Data/Skill/RPGSkillData.h"
#include "Data/Text/RPGTextManager.h"
#include "Dialogue/RPGDialogueManager.h"
#include "Kismet/GameplayStatics.h"
#include "LevelChange/RPGLevelChangeManager.h"
#include "ObjectPool/RPGPoolManager.h"
#include "Sequence/RPGSequenceManager.h"
#include "UI/RPGHUD.h"
#include "UI/RPGMainWidget.h"
#include "UI/Tooltip/RPGTooltipManager.h"
#include "UI/Tooltip/TooltipContent/RPGTooltipContent.h"


const FRPGQuestDefinition* RPGHelper::GetQuestDefinition_Safe(const UObject* WorldContextObject, const FName& QuestId)
{
	if (WorldContextObject == nullptr)
		return nullptr;
	
	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		 return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;

	const URPGQuestDatabase* QuestDB = GameInstance->GetSubsystem<URPGQuestDatabase>();
	if (QuestDB == nullptr)
		return nullptr;

	return QuestDB->GetQuestDefinitionByName(QuestId);
}

const FRPGItemDefinition* RPGHelper::GetItemDefinition_Safe(const UObject* WorldContextObject, const FName& ItemId)
{
	if (WorldContextObject == nullptr)
		return nullptr;
	
	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;

	const URPGItemDatabase* ItemDB = GameInstance->GetSubsystem<URPGItemDatabase>();
	if (ItemDB == nullptr)
		return nullptr;

	return ItemDB->GetItemDataByName(ItemId);
}

const FRPGNpcDataRow* RPGHelper::GetNpcData_Safe(const UObject* WorldContextObject, const FName& NpcId)
{
	if (NpcId == NAME_None)
		return nullptr;
	
	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;

	const URPGNpcDatabase* NpcDatabase = GameInstance->GetSubsystem<URPGNpcDatabase>();
	if (NpcDatabase == nullptr)
		return nullptr;

	return NpcDatabase->GetNpcDataRowByName(NpcId);
}

const FRPGShopDataRow* RPGHelper::GetShopData_Safe(const UObject* WorldContextObject, const FName& ShopId)
{
	if (ShopId == NAME_None)
		return nullptr;
	
	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;

	const URPGShopDatabase* ShopDatabase = GameInstance->GetSubsystem<URPGShopDatabase>();
	if (ShopDatabase == nullptr)
		return nullptr;

	return ShopDatabase->GetShopDataById(ShopId);
}

const FRPGSkillDataRow* RPGHelper::GetSkillData_Safe(const UObject* WorldContextObject, const FGameplayTag& SkillTag)
{
	if (SkillTag.IsValid() == false)
		return nullptr;
	
	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;

	const URPGSkillDatabase* SkillDatabase = GameInstance->GetSubsystem<URPGSkillDatabase>();
	if (SkillDatabase == nullptr)
		return nullptr;

	return SkillDatabase->GetSkillDataByTag(SkillTag);
}

const FRPGEnemyDataRow* RPGHelper::GetEnemyDataRow_Safe(const UObject* WorldContextObject, const FName& EnemyId)
{
	if (EnemyId == NAME_None)
		return nullptr;
	
	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;

	URPGEnemyDatabase* EnemyDB = GameInstance->GetSubsystem<URPGEnemyDatabase>();
	if (EnemyDB == nullptr)
		return nullptr;
	
	return EnemyDB->GetEnemyData(EnemyId);
}


const URPGTooltipContent* RPGHelper::GetItemTooltipContent(const UObject* WorldContextObject, const FName& ItemId)
{
	const FRPGItemDefinition* ItemDef = GetItemDefinition_Safe(WorldContextObject, ItemId);
	if (ItemDef == nullptr)
		return nullptr;

	URPGItemTooltipContent* ItemTooltipContent = NewObject<URPGItemTooltipContent>();
	if (ItemTooltipContent == nullptr)
		return nullptr;
	
	if (ItemDef->ItemType == EItemType::Consume)
	{
		ItemTooltipContent->TooltipType = ERPGTooltipType::Consume;
	}
	else if (ItemDef->ItemType == EItemType::Equipment)
	{
		ItemTooltipContent->TooltipType = ERPGTooltipType::EquipmentSample;
	}
	ItemTooltipContent->ItemId = ItemId;
	

	return ItemTooltipContent;
}

URPGQuestManager* RPGHelper::GetQuestManager(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		return nullptr;
	
	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;

	return GameInstance->GetSubsystem<URPGQuestManager>();
}

URPGTooltipManager* RPGHelper::GetTooltipManager(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		return nullptr;

	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;
	
	return GameInstance->GetSubsystem<URPGTooltipManager>();
}

URPGShopManager* RPGHelper::GetShopManager(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		return nullptr;

	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;
	
	return GameInstance->GetSubsystem<URPGShopManager>();
}

URPGPoolManager* RPGHelper::GetPoolManager(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		return nullptr;

	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;
	
	return GameInstance->GetSubsystem<URPGPoolManager>();
}

URPGDialogueManager* RPGHelper::GetDialogueManager(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		return nullptr;

	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;
	
	return GameInstance->GetSubsystem<URPGDialogueManager>();
}

URPGPlayerDataManager* RPGHelper::GetPlayerDataManager(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		return nullptr;

	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;
	
	return GameInstance->GetSubsystem<URPGPlayerDataManager>();
}

URPGTextManager* RPGHelper::GetTextManager(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		return nullptr;

	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;
	
	return GameInstance->GetSubsystem<URPGTextManager>();
}

URPGLevelChangeManager* RPGHelper::GetLevelChangeManager(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		return nullptr;

	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;
	
	return GameInstance->GetSubsystem<URPGLevelChangeManager>();
}

URPGSequenceManager* RPGHelper::GetSequenceManager(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		return nullptr;

	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	const auto* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;
	
	return GameInstance->GetSubsystem<URPGSequenceManager>();
}

ARPGPlayerCharacter* RPGHelper::GetPlayerCharacter_Safe(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		return nullptr;

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0);
	if (Character == nullptr)
		return nullptr;
	
	return Cast<ARPGPlayerCharacter>(Character);
}

ARPGPlayerState* RPGHelper::GetPlayerState_Safe(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		return nullptr;

	auto* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PC == nullptr)
		return nullptr;

	return PC->GetPlayerState<ARPGPlayerState>();
}

ARPGHUD* RPGHelper::GetHUD_Safe(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		return nullptr;

	auto* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PC == nullptr)
		return nullptr;

	return Cast<ARPGHUD>(PC->GetHUD());
}

UBlackboardComponent* RPGHelper::GetBlackboardComponent_Safe(ACharacter* AICharacter)
{
	AAIController* AIController = Cast<AAIController>(AICharacter->GetController());
	if (AIController == nullptr)
		return nullptr;
	
	return AIController->GetBlackboardComponent();
}


URPGAbilitySystemComponent* RPGHelper::GetPlayerAbilitySystemComponent_Safe(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		return nullptr;

	auto* PlayerState = GetPlayerState_Safe(WorldContextObject);
	if (PlayerState == nullptr)
		return nullptr;

	return Cast<URPGAbilitySystemComponent>(PlayerState->GetAbilitySystemComponent());
}

bool RPGHelper::GetMousePosition(const UObject* WorldContextObject, FVector2D& MousePosition)
{
	if (WorldContextObject == nullptr)
		return false;

	auto* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PC == nullptr)
		return false;

	return PC->GetMousePosition(MousePosition.X, MousePosition.Y);
}

bool RPGHelper::IsFriend(const AActor* FirstActor, const AActor* SecondActor)
{
	if (FirstActor == nullptr || SecondActor == nullptr)
		return false;

	const bool bBothArePlayers = FirstActor->ActorHasTag("Player") && SecondActor->ActorHasTag("Player");
	const bool bBothAreEnemies = FirstActor->ActorHasTag("Enemy") && SecondActor->ActorHasTag("Enemy");
	
	return bBothArePlayers || bBothAreEnemies;
}

bool RPGHelper::IsPlayerActor(const AActor* Actor)
{
	if (Actor == nullptr)
		return false;

	return Actor->ActorHasTag("Player");
}

FVector RPGHelper::GetActorFootLocation(AActor* Actor)
{
	if (Actor == nullptr)
		return FVector::ZeroVector;

	ACharacter* Character = Cast<ACharacter>(Actor);
	if (Character == nullptr)
		return FVector::ZeroVector;

	UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
	if (CapsuleComponent == nullptr)
		return FVector::ZeroVector;

	FVector FootLocation = Actor->GetActorLocation();
	FootLocation.Z -= CapsuleComponent->GetScaledCapsuleHalfHeight();
	return FootLocation;
}

UUserWidget* RPGHelper::GetWidgetInMainWidgetFromName(const UObject* WorldContextObject, FName WidgetName)
{
	ARPGHUD* HUD = RPGHelper::GetHUD_Safe(WorldContextObject);
	if (HUD == nullptr)
		return nullptr;

	URPGMainWidget* MainWidget =  HUD->GetMainWidget();
	if (MainWidget == nullptr)
		return nullptr;
	
	return Cast<UUserWidget>(MainWidget->GetWidgetFromName(WidgetName));
}

FName RPGHelper::ArmorTypeToName(EArmorType ArmorType)
{
	switch (ArmorType) {
	case EArmorType::Helmets:
		return FName("Helmets");
	case EArmorType::Top:
		return FName("Top");
	case EArmorType::Bottom:
		return FName("Bottom");
	case EArmorType::Shoes:
		return FName("Shoes");
	}

	return NAME_None;
}

void RPGHelper::SetBlackboardValueAsFloat(APawn* OwnerPawn, FName BlackboardKey, float Value)
{
	if (AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController()))
	{
		if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())
		{
			Blackboard->SetValueAsFloat(BlackboardKey, Value);
		}
	}
}

void RPGHelper::SetBlackboardValueAsBool(APawn* OwnerPawn, FName BlackboardKey, bool Value)
{
	if (AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController()))
	{
		if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())
		{
			Blackboard->SetValueAsBool(BlackboardKey, Value);
		}
	}
}




