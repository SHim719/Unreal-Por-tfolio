
#pragma once

#include "CoreMinimal.h"
#include "Player/RPGPlayerState.h"
#include "Shop/RPGShopManager.h"
#include "UI/RPGHUD.h"


class UBlackboardComponent;
class URPGTextManager;
class URPGPlayerDataManager;
class URPGDialogueManager;
class URPGPoolManager;
struct FRPGEnemyDataRow;
struct FRPGSkillDataRow;
struct FRPGShopDataRow;
struct FRPGNpcDataRow;
class ARPGPlayerCharacter;
class URPGTooltipContent;
struct FRPGItemOptionRow;
class URPGTooltipManager;
struct FRPGItemDefinition;
struct FRPGQuestDefinition;
class URPGQuestManager;
class ACharacter;


namespace RPGHelper 
{
	const FRPGQuestDefinition* GetQuestDefinition_Safe(const UObject* WorldContextObject, const FName& QuestId);
	const FRPGItemDefinition* GetItemDefinition_Safe(const UObject* WorldContextObject, const FName& ItemId);
	const FRPGNpcDataRow* GetNpcData_Safe(const UObject* WorldContextObject, const FName& NpcId);
	const FRPGShopDataRow* GetShopData_Safe(const UObject* WorldContextObject, const FName& ShopId);
	const FRPGSkillDataRow* GetSkillData_Safe(const UObject* WorldContextObject, const FGameplayTag& SkillTag);
	const FRPGEnemyDataRow* GetEnemyDataRow_Safe(const UObject* WorldContextObject, const FName& EnemyId);
	
	const URPGTooltipContent* GetItemTooltipContent(const UObject* WorldContextObject, const FName& ItemId);
	
	URPGQuestManager* GetQuestManager(const UObject* WorldContextObject);
	URPGTooltipManager* GetTooltipManager(const UObject* WorldContextObject);
	URPGShopManager* GetShopManager(const UObject* WorldContextObject);
	URPGPoolManager* GetPoolManager(const UObject* WorldContextObject);
	URPGDialogueManager* GetDialogueManager(const UObject* WorldContextObject);
	URPGPlayerDataManager* GetPlayerDataManager(const UObject* WorldContextObject);
	URPGTextManager* GetTextManager(const UObject* WorldContextObject);

	ARPGPlayerCharacter* GetPlayerCharacter_Safe(const UObject* WorldContextObject);
	ARPGPlayerState* GetPlayerState_Safe(const UObject* WorldContextObject);
	ARPGHUD* GetHUD_Safe(const UObject* WorldContextObject);

	UBlackboardComponent* GetBlackboardComponent_Safe(ACharacter* AICharacter);
	void SetBlackboardValueAsFloat(APawn* OwnerPawn, FName BlackboardKey, float Value);
	void SetBlackboardValueAsBool(APawn* OwnerPawn, FName BlackboardKey, bool Value);

	URPGAbilitySystemComponent* GetPlayerAbilitySystemComponent_Safe(const UObject* WorldContextObject);
	
	bool GetMousePosition(const UObject* WorldContextObject, FVector2D& MousePosition);
	bool IsFriend(const AActor* FirstActor, const AActor* SecondActor);
	bool IsPlayerActor(const AActor* Actor);

	FVector GetActorFootLocation(AActor* Actor);

	UUserWidget* GetWidgetInMainWidgetFromName(const UObject* WorldContextObject, FName WidgetName);
	
	FName ArmorTypeToName(EArmorType ArmorType);
}
