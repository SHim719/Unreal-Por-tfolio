#include "UI/Npc/RPGNpcDescTextWidget.h"

#include "Components/TextBlock.h"

void URPGNpcDescTextWidget::SetNpcName(const FText& NpcName) const
{
	if (Text_NpcName)
	{
		Text_NpcName->SetText(NpcName);
	}
	
}

void URPGNpcDescTextWidget::UpdateQuestMarker(const EQuestMarkerState State) const
{
	if (Text_QuestMarker == nullptr)
		return;
	
	switch (State) {
	case EQuestMarkerState::None:
		Text_QuestMarker->SetText(FText::GetEmpty());
		break;
	case EQuestMarkerState::HaveQuest:
		Text_QuestMarker->SetText(FText::FromString("!"));
		break;
	case EQuestMarkerState::WaitComplete:
		Text_QuestMarker->SetText(FText::FromString("?"));
		break;
	}

}
