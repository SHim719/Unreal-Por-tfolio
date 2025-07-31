// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGDialogueBox.generated.h"

struct FUpdateDialogueBoxMsg;
struct FRPGDialogueLine;
class UButton;
class UTextBlock;
struct FGameplayTag;
struct FOpenDialogueBoxMsg;

UCLASS()
class RPGPROJECT_API URPGDialogueBox : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	void Init(const FText& NpcNameText, const FText& DialogueText);

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Exit;

private:
	void HandleUpdateDialogueMsg(FGameplayTag, const FUpdateDialogueBoxMsg& Msg);

	UFUNCTION()
	void OnClick_ButtonNext();
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Speaker;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Dialogue;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Next;
	

};
