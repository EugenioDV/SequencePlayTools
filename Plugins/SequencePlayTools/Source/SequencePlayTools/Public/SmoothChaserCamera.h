// SmoothChaserCamera.h
#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "CineCameraComponent.h"
#include "CineCameraSettings.h"
#include "SmoothChaserCamera.generated.h"

struct FCameraSettingsSnapshot
{
	FCameraFilmbackSettings FilmbackSettings;
	FCameraLensSettings LensSettings;
	FCameraFocusSettings FocusSettings;
	FPlateCropSettings CropSettings;
	float CurrentFocalLength;
	float CurrentAperture;
	bool bOverride_CustomNearClippingPlane;
	float CustomNearClippingPlane;

	FCameraSettingsSnapshot() : CurrentFocalLength(0), CurrentAperture(0), bOverride_CustomNearClippingPlane(false), CustomNearClippingPlane(0) {}

	FCameraSettingsSnapshot(const UCineCameraComponent* Camera)
	{
		if (IsValid(Camera)) return;

		FilmbackSettings = Camera->Filmback;
		LensSettings = Camera->LensSettings;
		FocusSettings = Camera->FocusSettings;
		CropSettings = Camera->CropSettings;
		CurrentFocalLength = Camera->CurrentFocalLength;
		CurrentAperture = Camera->CurrentAperture;
		bOverride_CustomNearClippingPlane = Camera->bOverride_CustomNearClippingPlane;
		CustomNearClippingPlane = Camera->CustomNearClippingPlane;
	}
};

UCLASS()
class SEQUENCEPLAYTOOLS_API ASmoothChaserCamera : public ACineCameraActor
{
	GENERATED_BODY()

public:

	//todo linetrace to see if there's stuff in the middle
	//todo timeline length (bp) should be based on distance with min and max and a multiplier
	//todo camera snap conditions (e.g. too far, linetrace hit, etc)
	UFUNCTION(BlueprintCallable, Category = "Smooth Chaser Camera")
	void SetTargetCamera(ACineCameraActor* NewTargetCamera, bool bSnapImmediately = false, float InterpolationTime = 2.f);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Smooth Chaser Camera")
	void OnTargetCameraChanged(bool bSnapImmediately = false, float InterpolationTime = 2.f);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Smooth Chaser Camera")
	ACineCameraActor* TargetCamera = nullptr;

	FCameraSettingsSnapshot CameraSnapshot;

	UFUNCTION(BlueprintCallable, Category = "Smooth Chaser Camera")
   	void LerpCameraParams(float Alpha);
};
