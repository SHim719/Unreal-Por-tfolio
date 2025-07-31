#include "RPGConsumeAttribute.h"

#include "Kismet/GameplayStatics.h"


void URPGConsumeAttribute::PlayConsumeSound(UObject* WorldContextObject) const
{
	if (WorldContextObject == nullptr)
		return;
	
	if (SoundWhenConsumed)
	{
		UGameplayStatics::PlaySound2D(WorldContextObject->GetWorld(), SoundWhenConsumed);
	}
}
