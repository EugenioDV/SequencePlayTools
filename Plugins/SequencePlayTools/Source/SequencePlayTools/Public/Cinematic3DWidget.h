// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Cinematic3DWidget.generated.h"

UENUM(BlueprintType)
enum class ECinematicWidgetSpace : uint8
{
	Undefined,
	/** The widget is rendered in the world as mesh, it can be occluded like any other mesh in the world. */
	World,
	/** The widget is rendered in the screen, completely outside of the world, never occluded. */
	Screen
};


UCLASS()
class SEQUENCEPLAYTOOLS_API ACinematic3DWidget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACinematic3DWidget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Returns all widgets able to display text in the hierarchy of the Widget Component
	UFUNCTION(BlueprintCallable, Category = "Cinematic 3D Widget")
	TArray<UWidget*> GetAllTextWidgets();

protected:

	/** The widget component for this actor */
	UPROPERTY(Category = Cinematic3DWidget, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> WidgetComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cinematic3DWidget", Interp, meta=(DisplayName = "Opacity"))
	float Opacity = 1.f;

	// Opacity for all text elements in the widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cinematic3DWidget", Interp, meta=(DisplayName = "Text Opacity"))
	float TextOpacity = 1.f;

	// UserWidget class reference to be used for spawnables (Component variable in spawnnables won't work)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cinematic3DWidget Spawnable Setup Essentials", meta=(DisplayName = "Widget Class"))
	TSubclassOf<UUserWidget> WidgetClass;

	// World or screen, leave blank to leave the component setting (only set this for spawnables as the component variables don't work)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cinematic3DWidget Spawnable Setup Essentials", meta=(DisplayName = "Widget Render Space"))
	ECinematicWidgetSpace  WidgetSpace = ECinematicWidgetSpace::Undefined;

};
