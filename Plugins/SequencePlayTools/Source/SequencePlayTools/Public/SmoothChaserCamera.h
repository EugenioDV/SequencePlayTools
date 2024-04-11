// SmoothChaserCamera.h
#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "SmoothChaserCamera.generated.h"

UCLASS()
class SEQUENCEPLAYTOOLS_API ASmoothChaserCamera : public ACineCameraActor
{
	GENERATED_BODY()

public:

	//todo linetrace to see if there's stuff in the middle
	//todo timeline length (bp) should be based on distance with min and max and a multiplier
	//todo camera snap conditions (e.g. too far, linetrace hit, etc)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Smooth Chaser Camera")
	void SetTargetCamera(ACineCameraActor* NewTargetCamera, bool bSnapImmediately = false, float InterpolationTime = 2.f);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Smooth Chaser Camera")
	ACineCameraActor* TargetCamera = nullptr;
	
};
