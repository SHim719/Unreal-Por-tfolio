

#pragma once

#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTagContainer.h"
#include "RPGDefines.h"
#include "Components/RPGInventoryComponent.h"
#include "Data/Dialogue/RPGDialogueData.h"
#include "Quest/RPGQuestInstance.h"
#include "Quest/QuestTask/RPGQuestTask.h"
#include "RPGGameMessage.generated.h"

class URPGSkillTooltipContent;
class URPGSkillTreeData;
class URPGItemInstance;
class URPGTooltipContent;

namespace RPGMessage
{
	template<typename Message>
	void BroadcastMessage(UObject* WorldContextObject, const Message& InMessage, FGameplayTag ChannelTag)
	{
		if (WorldContextObject == nullptr)
			return;

		UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(WorldContextObject);
		MessageSystem.BroadcastMessage(ChannelTag, InMessage);
	}

	template <typename FMessageStructType, typename TOwner = UObject>
	FGameplayMessageListenerHandle RegisterListener(UObject* WorldContextObject, FGameplayTag Channel, TOwner* Object, void(TOwner::* Function)(FGameplayTag, const FMessageStructType&))
	{
		if (WorldContextObject == nullptr)
			return FGameplayMessageListenerHandle();

		UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(WorldContextObject);
		FGameplayMessageListenerHandle ListenerHandle = MessageSystem.RegisterListener<FMessageStructType>(Channel, Object, Function);
		return ListenerHandle;
	}

}

USTRUCT()
struct FInitInventorySizeMsg
{
	GENERATED_BODY()
	
	FInitInventorySizeMsg() {}
	
	FInitInventorySizeMsg(const int32 InInventorySize)
		: InventorySize(InInventorySize) {}	

	int32 InventorySize = 0;
};

USTRUCT()
struct FUpdateInventorySlotMsg
{
	GENERATED_BODY()

	FUpdateInventorySlotMsg() {}
	
	FUpdateInventorySlotMsg(EItemType _ItemType, int32 _SlotIndex, class UTexture2D* _Thumbnail, int32 _ItemCount, URPGTooltipContent* _TooltipContent)
		: ItemType(_ItemType), SlotIndex(_SlotIndex), Thumbnail(_Thumbnail), ItemCount(_ItemCount), TooltipContent(_TooltipContent) {}

	EItemType		ItemType = EItemType::End;
	int32			SlotIndex = 0;
	
	UPROPERTY()
	UTexture2D*		Thumbnail = nullptr;
	int32			ItemCount = 0;

	UPROPERTY()
	const URPGTooltipContent* TooltipContent = nullptr;
};


USTRUCT()
struct FGoldChangedMsg
{
	GENERATED_BODY()
	
	int32 Gold = 0;
};


USTRUCT()
struct FUpdateEquipmentSlotMsg
{
	GENERATED_BODY()

	FUpdateEquipmentSlotMsg() {}

	FUpdateEquipmentSlotMsg(int32 _SlotIndex, UTexture2D* _Thumbnail, const URPGTooltipContent* _TooltipContent)
		: SlotIndex(_SlotIndex), Thumbnail(_Thumbnail), TooltipContent(_TooltipContent)  {}
	
	int32			SlotIndex = 0;

	UPROPERTY()
	UTexture2D*		Thumbnail = nullptr;
	
	UPROPERTY()
	const URPGTooltipContent* TooltipContent = nullptr;
};


USTRUCT()
struct FUpdateQuickBarSlotMsg
{
	GENERATED_BODY()

	FUpdateQuickBarSlotMsg() {}

	FUpdateQuickBarSlotMsg(int32 _SlotIndex, UTexture2D* _Thumbnail, int32 _Count, const URPGTooltipContent* _TooltipContent)
		: SlotIndex(_SlotIndex),  Count(_Count), Thumbnail(_Thumbnail), TooltipContent(_TooltipContent)  {}
	
	int32			SlotIndex = 0;
	int32			Count = 0;
	
	UPROPERTY()
	UTexture2D*		Thumbnail = nullptr;
	
	UPROPERTY()
	const URPGTooltipContent*		TooltipContent = nullptr;
};

USTRUCT()
struct FUpdateQuickBarCooldownMsg
{
	GENERATED_BODY()

	FUpdateQuickBarCooldownMsg() {}

	FUpdateQuickBarCooldownMsg(int32 _SlotIndex, float _RemainingTime, float _CooldownTime)
		: SlotIndex(_SlotIndex), RemainingTime(_RemainingTime), CooldownTime(_CooldownTime)  {}
	
	int32			SlotIndex = 0;
	float			RemainingTime = 0.f;
	float			CooldownTime = 0.f;
};


USTRUCT()
struct FSlotInteractionMsg
{
	GENERATED_BODY()

	FSlotInteractionMsg() {}
	
	FSlotInteractionMsg(ESlotType _SlotType, int32 _SlotIndex)
		: SlotType(_SlotType), SlotIndex(_SlotIndex) {}

	ESlotType		SlotType = ESlotType::End;
	int32			SlotIndex = 0;
};

USTRUCT()
struct FDragDropMsg
{
	GENERATED_BODY()

	FDragDropMsg() {}
	
	FDragDropMsg(ESlotType _FromSlotType, int32 _FromSlotIndex, ESlotType _ToSlotType, int32 _ToSlotIndex)
		: FromSlotType(_FromSlotType), FromSlotIndex(_FromSlotIndex), ToSlotType(_ToSlotType), ToSlotIndex(_ToSlotIndex) {}

	ESlotType		FromSlotType = ESlotType::End;
	int32			FromSlotIndex = 0;
	ESlotType		ToSlotType = ESlotType::End;
	int32			ToSlotIndex = 0;
};

USTRUCT()
struct FInitializeSkillTreeWidgetMsg
{
	GENERATED_BODY()

	UPROPERTY()
	const URPGSkillTreeData* SkillTreeData = nullptr;

	int32 MaxColumn = 0;
};

USTRUCT()
struct FUpdateSkillTooltipMsg
{
	GENERATED_BODY()

	FGameplayTag SkillTag;
	
	UPROPERTY()
	const URPGSkillTooltipContent* TooltipContent = nullptr;
};

USTRUCT()
struct FOnClickSkillLevelUpMsg
{
	GENERATED_BODY()

	FOnClickSkillLevelUpMsg() {}
	FOnClickSkillLevelUpMsg(const FGameplayTag& InSkillTag) : SkillTag(InSkillTag) { }
	
	FGameplayTag SkillTag;
};

USTRUCT()
struct FStartNpcInteractionMsg
{
	GENERATED_BODY()

	FName				NpcId;
	
	TArray<FName> 		GivableQuestIds;
	TArray<FName>		WaitCompleteQuestIds;
};

USTRUCT()
struct FEndNpcInteractionMsg
{
	GENERATED_BODY()
	
};


USTRUCT()
struct FUpdateDialogueBoxMsg
{
	GENERATED_BODY()

	FText SpeakerText;
	FText DialogueText;
	bool bIsFinalDialogue = false;
};

USTRUCT()
struct FOnAcceptQuestMsg
{
	GENERATED_BODY()

	FOnAcceptQuestMsg() { }

	FOnAcceptQuestMsg(const FName _QuestId) : QuestId(_QuestId) {}

	FName QuestId;
};

USTRUCT()
struct FOnDropQuestMsg
{
	GENERATED_BODY()

	FOnDropQuestMsg() { }

	FOnDropQuestMsg(const FName _QuestId) : QuestId(_QuestId) {}

	FName QuestId;
};

USTRUCT()
struct FOnCompleteQuestMsg
{
	GENERATED_BODY()

	FOnCompleteQuestMsg() { }

	FOnCompleteQuestMsg(const FName _QuestId) : QuestId(_QuestId) {}
	
	FName QuestId;
};

USTRUCT()
struct FOnQuestStateChangeMsg
{
	GENERATED_BODY()

	FName QuestId;
	ERPGQuestState QuestState;
};


USTRUCT()
struct FQuestTaskSignalMsg
{
	GENERATED_BODY()
	
	ERPGQuestTaskActionType TaskActionType;
	FName TargetId;
	int32 Count = 0;
};

USTRUCT()
struct FUpdateNpcQuestListMsg
{
	GENERATED_BODY()
	
	FName NpcId;

	TArray<FName> GivableQuestIds;
	TArray<FName> WaitCompleteQuestIds;
};

USTRUCT()
struct FShowNpcQuestInfoMsg
{
	GENERATED_BODY()

	FName QuestId;
};

USTRUCT()
struct FAddOrRemoveSellItemMsg
{
	GENERATED_BODY()
	
	UPROPERTY()
	FRPGInventoryEntry SellEntry;
};

USTRUCT()
struct FOnPlayerChangeEquipmentMsg
{
	GENERATED_BODY()
	
	UPROPERTY()
	const URPGItemInstance* EquipItemInstance;

	bool bIsEquipped = false;
};











