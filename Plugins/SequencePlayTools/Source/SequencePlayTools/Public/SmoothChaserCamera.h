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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Blending Properties")
	void SetTargetCamera(ACineCameraActor* NewTargetCamera);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Blending Properties")
	ACineCameraActor* TargetCamera = nullptr;
	
};
