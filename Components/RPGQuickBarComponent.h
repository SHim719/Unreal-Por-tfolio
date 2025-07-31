

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RPGDefines.h"
#include "Components/ActorComponent.h"
#include "RPGQuickBarComponent.generated.h"

class URPGTooltipContent;
class URPGSkillTreeComponent;
class URPGInventoryComponent;
class URPGAbilitySystemComponent;

struct FRefSlotInfo
{
	ESlotType RefSlotType = ESlotType::End;
	int32 RefSlotIndex = 0;

	void Clear()
	{
		RefSlotType = ESlotType::End;
		RefSlotIndex = 0;
	}
};

struct FDragDropMsg;
DECLARE_DELEGATE_OneParam(FOnQuickBarKeyInputed, const int32 SlotIndex)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API URPGQuickBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URPGQuickBarComponent();
	void Initialize(const TArray<FRefSlotInfo>& InRefSlotInfos);
	
	void KeyInputStarted(const int32 SlotIndex);
	void KeyInputReleased(const int32 SlotIndex);

	const TArray<FRefSlotInfo>& GetRefSlotInfos() { return RefSlotInfos;}
private:
	void HandleDragDropMsg(FGameplayTag Tag, const FDragDropMsg& Msg);
	void HandleBindConsumeItem(const FDragDropMsg& Msg);
	void HandleBindSkill(const FDragDropMsg& Msg);
	void HandleSwapSlot(const FDragDropMsg& Msg);
	
	void BindConsumeEntryChanged(const int32 QuickSlotIndex, const int32 RefSlotIndex);
	void BindSkill(const int32 QuickSlotIndex);

	void HandleCooldownChange(float RemainingTime, float CooldownTime, const int32 QuickSlotIndex);

	void UpdateSlot(const int32 QuickSlotIndex);
	void UpdateSlotWidget(const int32 QuickSlotIndex, UTexture2D* Thumbnail, const int32 Count, const URPGTooltipContent* TooltipContent);
	
private:
	UPROPERTY()
	TObjectPtr<URPGAbilitySystemComponent> OwnerASC;

	UPROPERTY()
	TObjectPtr<URPGInventoryComponent> InventoryComponent;

	UPROPERTY()
	TObjectPtr<URPGSkillTreeComponent> SkillTreeComponent;
	
	TArray<FRefSlotInfo> RefSlotInfos;
	
	const int32 SlotCount = 5;
};




