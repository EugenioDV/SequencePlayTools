// SmoothChaserCamera.cpp
#include "SmoothChaserCamera.h"

void ASmoothChaserCamera::SetTargetCamera_Implementation(ACineCameraActor* NewTargetCamera, bool bSnapImmediately)
{
	TargetCamera = NewTargetCamera;
	UE_LOG(LogTemp, Warning, TEXT("Called native implementation that is supposed to be handled in blueprints for ASmoothChaserCamera"));
}
