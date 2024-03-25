// Copyright Epic Games, Inc. All Rights Reserved.

#include "SequencePlayToolsBPLibrary.h"

#include "EngineUtils.h"
#include "SequenceOrchestrator.h"

USequencePlayToolsBPLibrary::USequencePlayToolsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void USequencePlayToolsBPLibrary::PlaySequenceQueueItemAt(UObject* WorldContextObject, int32 QueueItemIndex)
{
	if (!WorldContextObject) return;

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return;

	// Find the first SequenceOrchestrator instance in the world
	for (TActorIterator<ASequenceOrchestrator> It(World); It; ++It)
	{
		if (ASequenceOrchestrator* Orchestrator = *It)
		{
			// Play the specified queue item
			Orchestrator->PlayQueueItem(QueueItemIndex);
			return;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("SequenceOrchestrator not found in the world."));
}
