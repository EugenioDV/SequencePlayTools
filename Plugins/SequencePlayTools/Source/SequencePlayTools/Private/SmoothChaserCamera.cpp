// SmoothChaserCamera.cpp
#include "SmoothChaserCamera.h"

void ASmoothChaserCamera::SetTargetCamera(ACineCameraActor* NewTargetCamera, bool bSnapImmediately, float InterpolationTime)
{
	TargetCamera = NewTargetCamera;
	CameraSnapshot = FCameraSettingsSnapshot(GetCineCameraComponent());
	OnTargetCameraChanged(bSnapImmediately, InterpolationTime);
}

void ASmoothChaserCamera::LerpCameraParams(float Alpha)
{
	if (!IsValid(TargetCamera) || !IsValid(TargetCamera->GetCineCameraComponent()))
	{
		UE_LOG(LogTemp, Warning, TEXT("LerpCameraParams: Target Camera invalid."));
		return;
	}

	// FilmbackSettings start
	FCameraFilmbackSettings TargetFilmback = TargetCamera->GetCineCameraComponent()->Filmback;

	TargetFilmback.SensorWidth = FMath::Lerp(CameraSnapshot.FilmbackSettings.SensorWidth, TargetFilmback.SensorWidth, Alpha);
	TargetFilmback.SensorHeight = FMath::Lerp(CameraSnapshot.FilmbackSettings.SensorHeight, TargetFilmback.SensorHeight, Alpha);
	TargetFilmback.SensorAspectRatio = FMath::Lerp(CameraSnapshot.FilmbackSettings.SensorAspectRatio, TargetFilmback.SensorAspectRatio, Alpha);

	// todo disabled GetCineCameraComponent()->SetFilmback(TargetFilmback);
	// FilmbackSettings end

	// LensSettings start
	FCameraLensSettings TargetLens = TargetCamera->GetCineCameraComponent()->LensSettings;
	
	TargetLens.MinFocalLength = FMath::Lerp(CameraSnapshot.LensSettings.MinFocalLength, TargetLens.MinFocalLength, Alpha);
	TargetLens.MaxFocalLength = FMath::Lerp(CameraSnapshot.LensSettings.MaxFocalLength, TargetLens.MaxFocalLength, Alpha);
	TargetLens.MinFStop = FMath::Lerp(CameraSnapshot.LensSettings.MinFStop, TargetLens.MinFStop, Alpha);
	TargetLens.MaxFStop = FMath::Lerp(CameraSnapshot.LensSettings.MaxFStop, TargetLens.MaxFStop, Alpha);
	TargetLens.MinimumFocusDistance = FMath::Lerp(CameraSnapshot.LensSettings.MinimumFocusDistance, TargetLens.MinimumFocusDistance, Alpha);
	TargetLens.SqueezeFactor = FMath::Lerp(CameraSnapshot.LensSettings.SqueezeFactor, TargetLens.SqueezeFactor, Alpha);
	//DiaphragmBladeCount is left out
	
	GetCineCameraComponent()->SetLensSettings(TargetLens);
	// LensSettings end
	
	// FocusSettings start
	FCameraFocusSettings TargetFocus = TargetCamera->GetCineCameraComponent()->FocusSettings;

	TargetFocus.ManualFocusDistance = FMath::Lerp(CameraSnapshot.FocusSettings.ManualFocusDistance, TargetFocus.ManualFocusDistance, Alpha);
	TargetFocus.FocusSmoothingInterpSpeed = FMath::Lerp(CameraSnapshot.FocusSettings.FocusSmoothingInterpSpeed, TargetFocus.FocusSmoothingInterpSpeed, Alpha);
	TargetFocus.FocusOffset = FMath::Lerp(CameraSnapshot.FocusSettings.FocusOffset, TargetFocus.FocusOffset, Alpha);

	GetCineCameraComponent()->SetFocusSettings(TargetFocus);
	// FocusSettings end
	
	//  CropSettings start
	FPlateCropSettings TargetCrop = TargetCamera->GetCineCameraComponent()->CropSettings;

	TargetCrop.AspectRatio = FMath::Lerp(CameraSnapshot.CropSettings.AspectRatio, TargetCrop.AspectRatio, Alpha);

	//todo disabled GetCineCameraComponent()->SetCropSettings(TargetCrop);
	// CropSettings end

	// Other params start
	GetCineCameraComponent()->bOverride_CustomNearClippingPlane = TargetCamera->GetCineCameraComponent()->bOverride_CustomNearClippingPlane;
	GetCineCameraComponent()->SetCurrentFocalLength(FMath::Lerp(CameraSnapshot.CurrentFocalLength, TargetCamera->GetCineCameraComponent()->CurrentFocalLength, Alpha));
	GetCineCameraComponent()->SetCurrentAperture(FMath::Lerp(CameraSnapshot.CurrentAperture, TargetCamera->GetCineCameraComponent()->CurrentAperture, Alpha));
	GetCineCameraComponent()->SetCustomNearClippingPlane(FMath::Lerp(CameraSnapshot.CustomNearClippingPlane, TargetCamera->GetCineCameraComponent()->CustomNearClippingPlane, Alpha));
	// Other params end

	GetCineCameraComponent()->MarkRenderStateDirty();
}

void ASmoothChaserCamera::OnTargetCameraChanged_Implementation(bool bSnapImmediately, float InterpolationTime)
{
	UE_LOG(LogTemp, Warning, TEXT("OnTargetCameraChanged::Called native implementation that is supposed to be handled in blueprints for ASmoothChaserCamera"));
}
