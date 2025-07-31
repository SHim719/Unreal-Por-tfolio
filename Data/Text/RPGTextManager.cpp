// 


#include "RPGTextManager.h"

#include "Internationalization/StringTable.h"

void URPGTextManager::Initialize(FSubsystemCollectionBase& Collection)
{
	UICommonTextTable = LoadObject<UStringTable>(GetGameInstance(),TEXT("/Game/Data/ST_UICommonTable"));
}

FText URPGTextManager::GetUIText(const FString& Key) const
{
	return FText::FromStringTable(UICommonTextTable->GetStringTableId(), Key);
}
