// Copyright Epic Games, Inc. All Rights Reserved.

#include "SequencePlayToolsBPLibrary.h"

#include "EngineUtils.h"
#include "SequenceOrchestrator.h"
#include "SmoothChaserCamera.h"

USequencePlayToolsBPLibrary::USequencePlayToolsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void USequencePlayToolsBPLibrary::PlaySequenceQueueItemAt(UObject* WorldContextObject, int32 QueueItemIndex)
{
	if (!WorldContextObject) return;

	const UWorld* World = WorldContextObject->GetWorld();
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

void USequencePlayToolsBPLibrary::SetSmoothChaserCameraTarget(UObject* WorldContextObject,
                                                              ACineCameraActor* NewTargetCamera, bool bSnapImmediately)
{
	if (!WorldContextObject) return;

	const UWorld* World = WorldContextObject->GetWorld();
	if (!World) return;

	// Find the first SequenceOrchestrator instance in the world
	for (TActorIterator<ASmoothChaserCamera> It(World); It; ++It)
	{
		if (ASmoothChaserCamera* SmoothChaserCamera = *It)
		{
			SmoothChaserCamera->SetTargetCamera(NewTargetCamera,bSnapImmediately);
			return;
		}
	}
}
