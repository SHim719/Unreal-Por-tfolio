#include "RPGDialogueManager.h"

#include "Data/Dialogue/RPGDialogueData.h"
#include "DialogueAction/RPGDialogueAction.h"
#include "Message/RPGGameMessage.h"


void URPGDialogueManager::StartDialogue(FText InCurrentNpcNameText, URPGDialogueData* DialogueData)
{
	if (DialogueData == nullptr || DialogueData->Dialogues.IsEmpty())
		return;

	CurrentDialogue = DialogueData;
	CurrentDialogueIndex = 0;

	CurrentNpcNameText = InCurrentNpcNameText;
	UpdateDialogueLine();
}

void URPGDialogueManager::UpdateDialogueLine()
{
	if (CurrentDialogue == nullptr)
		return;

	for (auto DialogueAction : CurrentDialogue->Dialogues[CurrentDialogueIndex].DialogueActions)
	{
		if (DialogueAction)
		{
			DialogueAction->Execute(this);
		}
	}
	
	UpdateDialogueBoxWidget();
	

	CurrentDialogueIndex = CurrentDialogue->Dialogues[CurrentDialogueIndex].NextDialogueIndex;

}

void URPGDialogueManager::UpdateDialogueBoxWidget()
{
	FUpdateDialogueBoxMsg Msg;
	Msg.SpeakerText = CurrentDialogue->Dialogues[CurrentDialogueIndex].bIsPlayerSpeak == true ? FText::FromString(TEXT("나")) : CurrentNpcNameText;
	Msg.DialogueText = CurrentDialogue->Dialogues[CurrentDialogueIndex].Dialogue;
	Msg.bIsFinalDialogue = CurrentDialogue->Dialogues[CurrentDialogueIndex].NextDialogueIndex == -1;

	RPGMessage::BroadcastMessage(this, Msg, FGameplayTag::RequestGameplayTag(FName("Widget.Dialogue.UpdateDialogue")));
}
