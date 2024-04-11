// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "TargetCamera.generated.h"

/**
 * 
 */
UCLASS()
class SEQUENCEPLAYTOOLS_API ATargetCamera : public ACineCameraActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// The time it takes for the camera to reach target location, then it will snap, in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Camera", meta=(DisplayName = "Camera Interpolation Time"))
	float CameraInterpolationTime = 1.f;

	// Whether the chaser camera should snap immediately to this camera location or interpolate for CameraInterpolationTime seconds then snap
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Camera", meta=(DisplayName = "Snap Immediately"))
	bool bSnapImmediately = true;
	
};
