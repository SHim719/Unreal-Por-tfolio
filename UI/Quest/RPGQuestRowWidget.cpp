#include "RPGQuestRowWidget.h"

#include "RPGHelperLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Data/Quest/RPGQuestData.h"
#include "Data/Quest/RPGQuestDatabase.h"


void URPGQuestRowWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Quest)
	{
		Button_Quest->OnClicked.AddDynamic(this, &ThisClass::OnClickedButton);
	}
}

void URPGQuestRowWidget::SetRefQuestId(const FName& QuestId)
{
	RefQuestId = QuestId;
	
	if (const FRPGQuestDefinition* QuestDef = RPGHelper::GetQuestDefinition_Safe(this, QuestId))
	{
		if (Text_QuestTitle)
		{
			Text_QuestTitle->SetText(QuestDef->QuestTitle);
		}
	}
}

void URPGQuestRowWidget::OnClickedButton()
{
	OnClickedQuestRowButton.ExecuteIfBound(RefQuestId);
}
