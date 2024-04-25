#include "SequenceOrchestrator.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequencePlayer.h"

ASequenceOrchestrator::ASequenceOrchestrator()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASequenceOrchestrator::BeginPlay()
{
    Super::BeginPlay();
    
    if (bAutoPlayAtStart)
    {
        PlayQueueItem(QueueIndex);
    }
}

void ASequenceOrchestrator::PlayQueueItem(int32 QueueItemIndex)
{
    if (!SequenceQueue.IsValidIndex(QueueItemIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("ASequenceOrchestrator::PlayQueueItem: QueueItemIndex at index %i is out of range."), QueueItemIndex);
        return;
    }
    
    Resume();

    const FSequenceActorQueueItem& OldItemToStop = SequenceQueue[QueueIndex];
    if (IsValid(OldItemToStop.MainSequence) && IsValid(OldItemToStop.MainSequence->SequencePlayer))
    {
        OldItemToStop.MainSequence->SequencePlayer->OnFinished.RemoveAll(this);
        OldItemToStop.MainSequence->SequencePlayer->Stop();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ASequenceOrchestrator::PlayQueueItem: OldItemToStop MainSequence or SequencePlayer is invalid."));
    }

    QueueIndex = QueueItemIndex;
    
    const FSequenceActorQueueItem& NewItemToPlay = SequenceQueue[QueueItemIndex];
    if (IsValid(NewItemToPlay.MainSequence) && IsValid(NewItemToPlay.MainSequence->SequencePlayer))
    {
        NewItemToPlay.MainSequence->SequencePlayer->OnFinished.AddDynamic(this, &ASequenceOrchestrator::OnMainSequenceFinished);
        NewItemToPlay.MainSequence->SequencePlayer->Play();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ASequenceOrchestrator::PlayQueueItem: NewItemToPlay MainSequence or SequencePlayer at index %i is null."), QueueItemIndex);
    }

    UpdateSubSequences(NewItemToPlay.SubSequences);
}

void ASequenceOrchestrator::Pause()
{
    if (bIsPaused) return;
    bIsPaused = true;
    
    for (const auto LevelSequence : ActiveSubSequences)
    {
        if (IsValid(LevelSequence) && IsValid(LevelSequence->GetSequencePlayer()) && LevelSequence->GetSequencePlayer()->IsPlaying())
        {
            CachedPausedSequences.Add(LevelSequence);
            LevelSequence->GetSequencePlayer()->Pause();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ASequenceOrchestrator::Pause: LevelSequence or SequencePlayer is invalid."));
        }
    }
    
    if (IsValid(SequenceQueue[GetCurrentQueueIndex()].MainSequence) && IsValid(SequenceQueue[GetCurrentQueueIndex()].MainSequence->GetSequencePlayer()) && SequenceQueue[GetCurrentQueueIndex()].MainSequence->GetSequencePlayer()->IsPlaying())
    {
        CachedPausedSequences.Add(SequenceQueue[GetCurrentQueueIndex()].MainSequence);
        SequenceQueue[GetCurrentQueueIndex()].MainSequence->GetSequencePlayer()->Pause();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ASequenceOrchestrator::Pause: MainSequence or SequencePlayer at current index is invalid."));
    }
}

void ASequenceOrchestrator::Resume()
{
    bIsPaused = false;
    
    for (const auto LevelSequence : CachedPausedSequences)
    {
        if (IsValid(LevelSequence) && IsValid(LevelSequence->GetSequencePlayer()))
        {
            LevelSequence->GetSequencePlayer()->Play();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ASequenceOrchestrator::Resume: LevelSequence or SequencePlayer is invalid."));
        }
    }

    CachedPausedSequences.Reset();
}

void ASequenceOrchestrator::OnMainSequenceFinished()
{
    if (QueueIndex < SequenceQueue.Num() - 1)
    {
        if (SequenceQueue[QueueIndex].bAutoPlayNext)
        {
            PlayQueueItem(QueueIndex + 1);
        }
    }
    // Additional logic for handling the end of the queue or looping could be implemented here.
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
        if (!NextSubSequences.Contains(SubSequence) && IsValid(SubSequence) && IsValid(SubSequence->SequencePlayer))
        {
            SubSequence->SequencePlayer->Stop();
        }
    }
}

void ASequenceOrchestrator::StartNewSubSequences(const TArray<ALevelSequenceActor*>& NextSubSequences)
{
    for (ALevelSequenceActor* SubSequence : NextSubSequences)
    {
        if (!ActiveSubSequences.Contains(SubSequence) && IsValid(SubSequence) && IsValid(SubSequence->SequencePlayer))
        {
            SubSequence->SequencePlayer->Play();
        }
    }

    ActiveSubSequences = NextSubSequences;
}
