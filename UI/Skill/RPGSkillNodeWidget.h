

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Data/Skill/RPGSkillData.h"
#include "RPGSkillNodeWidget.generated.h"

class URPGTooltipContent;
struct FRPGSkillTreeNode;
class UGameplayEffect;
struct FOnAttributeChangeData;
class UButton;
class URPGSlotWidget;
class UTextBlock;

UCLASS()
class RPGPROJECT_API URPGSkillNodeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized()	override;
	void InitializeSkillSlot(const FRPGSkillTreeNode& SkillTreeNode, const int32 MaxColumn);
	
	void HandleSkillPointChanged(const int32 CurSkillPoint) const;
	void HandleSkillLevelChanged(const int32 CurSkillLevel, const bool IsMastered);
	void UpdateTooltip(const URPGTooltipContent* TooltipContent) const;

	bool IsActive() const { return bIsActive;}
private:
	void SetButtonActiveState(const bool bActive) const;
	
	UFUNCTION()
	void OnClick_SkillPointUpButton();

private:
	bool bIsMastered = false;
	bool bIsActive = false; // 스킬을 찍은 상태.
	bool bIsLearnable = false;
	FGameplayTag CachedSkillTag;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URPGSlotWidget> SlotWidget;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_CurPoint;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_MaxPoint;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Plus;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Button;
	
};


