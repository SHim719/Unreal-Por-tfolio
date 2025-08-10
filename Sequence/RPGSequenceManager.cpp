// 


#include "RPGSequenceManager.h"

#include "RPGHelperLibrary.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "UI/RPGMainWidget.h"

void URPGSequenceManager::Initialize(FSubsystemCollectionBase& Collection)
{
	FWorldDelegates::OnPreWorldFinishDestroy.AddUObject(this, &ThisClass::OnWorldDestroyed);
}

void URPGSequenceManager::PlaySequence(ULevelSequence* ToPlaySequence)
{
	if (ToPlaySequence == nullptr)
		return;

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->DisableInput(PC);
		OriginalViewTarget = PC->GetViewTarget();
	}
	
	
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bHideHud = true;
	PlaybackSettings.bHidePlayer = true;


	ALevelSequenceActor* SequenceActor;
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), ToPlaySequence, PlaybackSettings, SequenceActor);
	
	if (SequencePlayer)
	{
		SequencePlayer->OnFinished.AddDynamic(this, &URPGSequenceManager::SequenceFinished);
		SequencePlayer->Play();
	}

	if (ARPGHUD* HUD = RPGHelper::GetHUD_Safe(this))
	{
		if (URPGMainWidget* MainWidget = HUD->GetMainWidget())
		{
			MainWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	OnSequenceStarted.Broadcast();
}

void URPGSequenceManager::SequenceFinished()
{
	OnSequenceFinished.Broadcast();
	OnSequenceFinished.Clear();

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->EnableInput(PC);
		PC->SetViewTargetWithBlend(OriginalViewTarget, 0.5f);
		OriginalViewTarget = nullptr;
	}

	if (ARPGHUD* HUD = RPGHelper::GetHUD_Safe(this))
	{
		if (URPGMainWidget* MainWidget = HUD->GetMainWidget())
		{
			MainWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}

	SequencePlayer = nullptr;
}

void URPGSequenceManager::OnWorldDestroyed(UWorld* World)
{
	OnSequenceStarted.Clear();
	OnSequenceFinished.Clear();
}
