#include "RPGDialogueBox.h"

#include "RPGHelperLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Dialogue/RPGDialogueManager.h"
#include "Message/RPGGameMessage.h"

void URPGDialogueBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RPGMessage::RegisterListener(this, FGameplayTag::RequestGameplayTag(FName("Widget.Dialogue.UpdateDialogue")),
		this,&ThisClass::HandleUpdateDialogueMsg);

	if (Button_Next)
	{
		Button_Next->OnClicked.AddDynamic(this, &ThisClass::OnClick_ButtonNext);
	}
}

void URPGDialogueBox::Init(const FText& NpcNameText, const FText& DialogueText)
{
	if (Text_Speaker)
	{
		Text_Speaker->SetText(NpcNameText);
	}
	
	if (Text_Dialogue)
	{
		Text_Dialogue->SetText(DialogueText);
	}
	
	Button_Next->SetVisibility(ESlateVisibility::Collapsed);
}

void URPGDialogueBox::HandleUpdateDialogueMsg(FGameplayTag, const FUpdateDialogueBoxMsg& Msg)
{
	if (Text_Speaker)
	{
		Text_Speaker->SetText(Msg.SpeakerText);
	}

	if (Text_Dialogue)
	{
		Text_Dialogue->SetText(Msg.DialogueText);
	}

	if (Msg.bIsFinalDialogue)
	{
		Button_Next->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (Button_Next->GetVisibility() == ESlateVisibility::Collapsed)
	{
		Button_Next->SetVisibility(ESlateVisibility::Visible);
	}
}

void URPGDialogueBox::OnClick_ButtonNext()
{
	if (URPGDialogueManager* DialogueManager = RPGHelper::GetDialogueManager(this))
	{
		DialogueManager->UpdateDialogueLine();
	}
}

	




