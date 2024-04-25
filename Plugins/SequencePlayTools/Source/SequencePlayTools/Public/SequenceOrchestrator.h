// SequenceOrchestrator.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSequenceActor.h"
#include "SequenceOrchestrator.generated.h"

/**
 * Struct to store references to a main level sequence actor and an array of sub-level sequence actors.
 */
USTRUCT(BlueprintType)
struct FSequenceActorQueueItem
{
	GENERATED_BODY()

	// Reference to the main level sequence actor in the level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SequencePlayTools")
	ALevelSequenceActor* MainSequence = nullptr;
	
	// Array of references to sub-level sequence actors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SequencePlayTools")
	TArray<ALevelSequenceActor*> SubSequences;
	
	// Reference to the main level sequence actor in the level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SequencePlayTools")
	bool bAutoPlayNext = true;
};

UCLASS()
class ASequenceOrchestrator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASequenceOrchestrator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	int32 GetCurrentQueueIndex() const
	{
		return QueueIndex;
	}

	// Plays a single queue item
	void PlayQueueItem(int32 QueueItemIndex);

	void Pause();
	
	void Resume();

	// Called when the main sequence finishes
	UFUNCTION()
	void OnMainSequenceFinished();

	UFUNCTION(BlueprintCallable, Category = "Sequence Orchestrator")
	int32 GetQueueIndex(){ return QueueIndex; }

	// Updates the subsequences for the next queue item
	void UpdateSubSequences(const TArray<ALevelSequenceActor*>& NewSubSequences);

	// Array of sequence queue items
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence Orchestrator")
	TArray<FSequenceActorQueueItem> SequenceQueue;
	
	// Whether to automatically play the sequence queue on BeginPlay
	UPROPERTY(EditAnywhere, Category = "Sequence Orchestrator")
	bool bAutoPlayAtStart = true;
	
private:
	// Index of the currently playing item in the sequence queue, also works as starter index
	UPROPERTY(EditAnywhere, Category = "Sequence Orchestrator")
	int32 QueueIndex = 0;
	
	// Tracks active subsequences to manage transitions between queue items
	UPROPERTY()
	TArray<ALevelSequenceActor*> ActiveSubSequences;

	// Stops subsequences not present in the next queue item
	void StopMissingSubSequences(const TArray<ALevelSequenceActor*>& NextSubSequences);

	// Starts new subsequences present in the next queue item but not in the current
	void StartNewSubSequences(const TArray<ALevelSequenceActor*>& NextSubSequences);

	// When pause, the playing subsequences will be saved here
	UPROPERTY()
	TArray<ALevelSequenceActor*> CachedPausedSequences;

	bool bIsPaused = true;
};
