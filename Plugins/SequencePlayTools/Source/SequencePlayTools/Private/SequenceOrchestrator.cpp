// SequenceOrchestrator.cpp

#include "SequenceOrchestrator.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequencePlayer.h"

ASequenceOrchestrator::ASequenceOrchestrator()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
}

void ASequenceOrchestrator::BeginPlay()
{
    Super::BeginPlay();
    
    if (bAutoPlayAtStart)
    {
            PlayQueueItem(CurrentQueueIndex);
    }
}

void ASequenceOrchestrator::PlayQueueItem(int32 QueueItemIndex)
{
    if (!SequenceQueue.IsValidIndex(QueueItemIndex))
    {
        // Handle invalid index error. Could log an error or implement other error handling logic here.
        UE_LOG(LogTemp, Error, TEXT("SequenceOrchestrator: QueueItemIndex at index %i is out of range."), QueueItemIndex);
        return;
    }
    
    // Stop the currently playing sequence and unbind all from OnFinished
    const FSequenceActorQueueItem& OldItemToStop = SequenceQueue[CurrentQueueIndex];
    if (OldItemToStop.MainSequence && OldItemToStop.MainSequence->SequencePlayer)
    {
        OldItemToStop.MainSequence->SequencePlayer->OnFinished.RemoveAll(this);
        OldItemToStop.MainSequence->SequencePlayer->Stop();
    }

    CurrentQueueIndex = QueueItemIndex;
    
    const FSequenceActorQueueItem& NewItemToPlay = SequenceQueue[QueueItemIndex];
    if (NewItemToPlay.MainSequence  && NewItemToPlay.MainSequence->SequencePlayer)
    {
        NewItemToPlay.MainSequence->SequencePlayer->OnFinished.AddDynamic(this, &ASequenceOrchestrator::OnMainSequenceFinished);
        NewItemToPlay.MainSequence->SequencePlayer->Play();
    }
    else
    {
        // Handle error for missing main sequence
        UE_LOG(LogTemp, Error, TEXT("SequenceOrchestrator: MainSequence at index %i is null."), CurrentQueueIndex);
        return;
    }

    UpdateSubSequences(NewItemToPlay.SubSequences);
}

void ASequenceOrchestrator::OnMainSequenceFinished()
{
    
    if (CurrentQueueIndex < SequenceQueue.Num() - 1)
    {
        if (SequenceQueue[CurrentQueueIndex].bAutoPlayNext)
        {
            PlayQueueItem(CurrentQueueIndex + 1);
        }
    }
    else
    {
        // Reached the end of the queue, handle loop or completion here
    }
}

void ASequenceOrchestrator::UpdateSubSequences(const TArray<ALevelSequenceActor*>& NewSubSequences)
{
    StopMissingSubSequences(NewSubSequences);
    StartNewSubSequences(NewSubSequences);
}

void ASequenceOrchestrator::StopMissingSubSequences(const TArray<ALevelSequenceActor*>& NextSubSequences)
{
    for (ALevelSequenceActor* SubSequence : ActiveSubSequences)
    {
        if (!NextSubSequences.Contains(SubSequence))
        {
            // Stop the subsequence if it's not in the next set
            SubSequence->SequencePlayer->Stop();
        }
    }
}

void ASequenceOrchestrator::StartNewSubSequences(const TArray<ALevelSequenceActor*>& NextSubSequences)
{
    for (ALevelSequenceActor* SubSequence : NextSubSequences)
    {
        if (!ActiveSubSequences.Contains(SubSequence))
        {
            // Start the subsequence if it's new
            SubSequence->SequencePlayer->Play();
        }
    }

    // Update the active subsequences to the new set
    ActiveSubSequences = NextSubSequences;
}
