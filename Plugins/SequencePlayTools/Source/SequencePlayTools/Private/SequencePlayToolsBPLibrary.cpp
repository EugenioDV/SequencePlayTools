// Copyright Epic Games, Inc. All Rights Reserved.

#include "SequencePlayToolsBPLibrary.h"
#include "EngineUtils.h"
#include "SequenceOrchestrator.h"
#include "LevelSequence.h"
#include "SmoothChaserCamera.h"
#include "Tracks/MovieSceneEventTrack.h"


TMap<UMovieSceneSequence*, int32> USequencePlayToolsBPLibrary::SequenceLoopbackCounters;
TMap<UMovieSceneSequence*, FMovieSceneSequencePlaybackParams> USequencePlayToolsBPLibrary::SequenceLoopbackBookmarks;


USequencePlayToolsBPLibrary::USequencePlayToolsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

ASequenceOrchestrator* FindSequenceOrchestrator(const UObject* WorldContextObject)
{
    if (!WorldContextObject) return nullptr;

    const UWorld* World = WorldContextObject->GetWorld();
    if (!World) return nullptr;

    // Find the first SequenceOrchestrator instance in the world
    for (TActorIterator<ASequenceOrchestrator> It(World); It; ++It)
    {
        if (ASequenceOrchestrator* Orchestrator = *It)
        {
            return Orchestrator;
        }
    }
    return nullptr;
}

void USequencePlayToolsBPLibrary::PlaySequenceQueueItemAt(UObject* WorldContextObject, int32 QueueItemIndex)
{
    if (ASequenceOrchestrator* Orchestrator = FindSequenceOrchestrator(WorldContextObject))
    {
        Orchestrator->PlayQueueItem(QueueItemIndex);
        return;
    }

	UE_LOG(LogTemp, Error, TEXT("PlaySequenceQueueItemAt: SequenceOrchestrator not found in the world."));
}

void USequencePlayToolsBPLibrary::PlayNextSequence(UObject* WorldContextObject)
{
    if (ASequenceOrchestrator* Orchestrator = FindSequenceOrchestrator(WorldContextObject))
    {
        Orchestrator->PlayQueueItem(Orchestrator->GetCurrentQueueIndex()+1);
        return;
    }
    
    UE_LOG(LogTemp, Error, TEXT("PlayNextSequence: SequenceOrchestrator not found in the world."));
}

void USequencePlayToolsBPLibrary::PlayPreviousSequence(UObject* WorldContextObject)
{
    if (ASequenceOrchestrator* Orchestrator = FindSequenceOrchestrator(WorldContextObject))
    {
        Orchestrator->PlayQueueItem(Orchestrator->GetCurrentQueueIndex()-1);
        return;
    }
    
    UE_LOG(LogTemp, Error, TEXT("PlayPreviousSequence: SequenceOrchestrator not found in the world."));
}

void USequencePlayToolsBPLibrary::PauseOrchestrator(UObject* WorldContextObject)
{
    if (ASequenceOrchestrator* Orchestrator = FindSequenceOrchestrator(WorldContextObject))
    {
        Orchestrator->Pause();
        return;
    }
    
    UE_LOG(LogTemp, Error, TEXT("PauseOrchestrator: SequenceOrchestrator not found in the world."));
}

void USequencePlayToolsBPLibrary::ResumeOrchestrator(UObject* WorldContextObject)
{
    if (ASequenceOrchestrator* Orchestrator = FindSequenceOrchestrator(WorldContextObject))
    {
        Orchestrator->Resume();
        return;
    }
    
    UE_LOG(LogTemp, Error, TEXT("ResumeOrchestrator: SequenceOrchestrator not found in the world."));
}

void USequencePlayToolsBPLibrary::SetSmoothChaserCameraTarget(UObject* WorldContextObject,
                                                              ACineCameraActor* NewTargetCamera, bool bSnapImmediately, float InterpolationTime)
{
	if (!WorldContextObject) return;

	const UWorld* World = WorldContextObject->GetWorld();
	if (!World) return;

	// Find the first SequenceOrchestrator instance in the world
	for (TActorIterator<ASmoothChaserCamera> It(World); It; ++It)
	{
		if (ASmoothChaserCamera* SmoothChaserCamera = *It)
		{
			SmoothChaserCamera->SetTargetCamera(NewTargetCamera,bSnapImmediately,InterpolationTime);
			return;
		}
	}
}

void USequencePlayToolsBPLibrary::SaveLoopbackLocation(UObject* WorldContextObject, UMovieSceneSequence* LevelSequence,
    int32 MaxLoopbacks)
{
    if (!IsValid(LevelSequence))
    {
        UE_LOG(LogTemp, Warning, TEXT("SaveLoopbackLocation: LevelSequence is not valid."));
        return;
    }
    
    if (const ALevelSequenceActor* SequenceActor = FindUniqueSequenceActor(LevelSequence, WorldContextObject))
    {
        if (const ULevelSequencePlayer* SequencePlayer = SequenceActor->SequencePlayer)
        {
            const FQualifiedFrameTime BookmarkFrameTime = SequencePlayer->GetCurrentTime();
            FMovieSceneSequencePlaybackParams Bookmark;
            Bookmark.Time = BookmarkFrameTime.AsSeconds();
            Bookmark.PositionType = EMovieScenePositionType::Time;
            
            SequenceLoopbackBookmarks.Add(LevelSequence, Bookmark);
            SequenceLoopbackCounters.Add(LevelSequence, MaxLoopbacks);
        }
    }

    const UMovieScene* MovieScene = LevelSequence->GetMovieScene();
    if (!IsValid(MovieScene))
    {
        UE_LOG(LogTemp, Warning, TEXT("SaveLoopbackLocation: The MovieScene is invalid."));
        return;
    }

    const TArray<UMovieSceneTrack*>& Tracks = MovieScene->GetTracks();
    for (UMovieSceneTrack* Track : Tracks)
    {
        if (UMovieSceneEventTrack* EventTrack = Cast<UMovieSceneEventTrack>(Track))
        {
            if (EventTrack->bFireEventsWhenBackwards)
            {
                EventTrack->bFireEventsWhenBackwards = false;

                UE_LOG(LogTemp, Log, TEXT("SaveLoopbackLocation: Set 'Fire Events When Backwards' to false for EventTrack %s"), *EventTrack->GetTrackName().ToString());
            }
        }
    }
}

void USequencePlayToolsBPLibrary::TriggerSequenceLoopback(UObject* WorldContextObject, UMovieSceneSequence* LevelSequence)
{
    if (!IsValid(LevelSequence))
    {
        UE_LOG(LogTemp, Warning, TEXT("TriggerSequenceLoopback: LevelSequence is not valid."));
        return;
    }
    if (!SequenceLoopbackCounters.Contains(LevelSequence))
    {
        UE_LOG(LogTemp, Warning, TEXT("TriggerSequenceLoopback: LevelSequence is not in the loopback counters list."));
        return;
    }
    int32* Counter = SequenceLoopbackCounters.Find(LevelSequence);
    if (*Counter == 0)
    {
        UE_LOG(LogTemp, Verbose, TEXT("TriggerSequenceLoopback: Loopback counter for LevelSequence has reached zero."));
        // Consider whether you need to reset the counter here or take another action
        return;
    }

    if (const ALevelSequenceActor* SequenceActor = FindUniqueSequenceActor(LevelSequence, WorldContextObject))
    {
        if (ULevelSequencePlayer* SequencePlayer = SequenceActor->SequencePlayer)
        {
            if (const FMovieSceneSequencePlaybackParams* Bookmark = SequenceLoopbackBookmarks.Find(LevelSequence))
            {
                SequencePlayer->SetPlaybackPosition(*Bookmark);
                (*Counter)--;
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("TriggerSequenceLoopback: No bookmark found for LevelSequence."));
            }
        }
    }
}

ALevelSequenceActor* USequencePlayToolsBPLibrary::FindUniqueSequenceActor(const UMovieSceneSequence* LevelSequence, const UObject* WorldContextObject)
{
    if (!IsValid(LevelSequence)) 
    {
        UE_LOG(LogTemp, Error, TEXT("FindUniqueSequenceActor: LevelSequence is not valid."));
        return nullptr;
    }
    if (!WorldContextObject) 
    {
        UE_LOG(LogTemp, Error, TEXT("FindUniqueSequenceActor: WorldContextObject is null."));
        return nullptr;
    }
    const UWorld* World = WorldContextObject->GetWorld();
    if (!World) 
    {
        UE_LOG(LogTemp, Error, TEXT("FindUniqueSequenceActor: World is null."));
        return nullptr;
    }

    ALevelSequenceActor* SequenceOwner = nullptr;
    for (TActorIterator<ALevelSequenceActor> It(World); It; ++It)
    {
        if (ALevelSequenceActor* LevelSequenceActor = *It)
        {
            if (LevelSequenceActor->GetSequence() == LevelSequence)
            {
                if (SequenceOwner == nullptr)
                {
                    SequenceOwner = LevelSequenceActor;
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("FindUniqueSequenceActor: Multiple ALevelSequenceActor instances found for the given LevelSequence."));
                    return nullptr;
                }
            }
        }
    }
    if (!IsValid(SequenceOwner))
    {
        UE_LOG(LogTemp, Warning, TEXT("FindUniqueSequenceActor: No ALevelSequenceActor instance found for the given LevelSequence."));
    }
    return SequenceOwner;
}
