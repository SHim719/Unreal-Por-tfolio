

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/Skill/RPGSkillData.h"
#include "UI/Common/RPGMovableWidget.h"
#include "RPGSkillTreeWidget.generated.h"

struct FUpdateSkillTooltipMsg;
struct FInitializeSkillTreeWidgetMsg;
class UUniformGridPanel;
class URPGSkillNodeWidget;
class UTextBlock;




UCLASS()
class RPGPROJECT_API URPGSkillTreeWidget : public URPGMovableWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
	void BindDelegates();
	void MakeSkillTreeGraph(const URPGSkillTreeData* SkillTreeData);
	
	void HandleInitSkillTreeMsg(FGameplayTag Tag, const FInitializeSkillTreeWidgetMsg& Msg);
	void HandleUpdateTooltipMsg(FGameplayTag, const FUpdateSkillTooltipMsg& Msg);
	void HandleSkillPointChanged(const int32 NewSkillPoint) const;
	void HandleSkillLevelChanged(const FGameplayTag& SkillTag, const int32 NewSkillLevel);

	URPGSkillNodeWidget* GetSkillNodeWidgetByTag(const FGameplayTag& SkillTag) const;

	FVector2D GetSkillNodeWidgetAbsolutePos(const FGameplayTag& Tag) const;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SkillPoint;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UUniformGridPanel> Grid_SkillTree;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPGSkillNodeWidget>  SkillSlotWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	int32 DrawLineLayerOffset;

	UPROPERTY()
	TMap<FGameplayTag, FRPGSkillTreeNode> CachedSkillTreeNodes;
	
	// 스킬 트리 라인을 렌더링하기 위한 그래프 정보(스킬 태그, 이 스킬와 연결 활성화 여부(조건 충족되었을 때))
	TMap<FGameplayTag, TArray<FGameplayTag>> SkillTreeGraph;
	//TMap<FGameplayTag, TPair<int32, int32>>
};
