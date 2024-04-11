// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetCamera.h"
#include "SequencePlayToolsBPLibrary.h"

void ATargetCamera::BeginPlay()
{
	Super::BeginPlay();

	USequencePlayToolsBPLibrary::SetSmoothChaserCameraTarget(this, this, bSnapImmediately, CameraInterpolationTime);
	
}
