// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Cinematic3DWidget.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cinematic3DWidget", Interp, meta=(DisplayName = "Text Opacity"))
	float TextOpacity = 1.f;

	// UserWidget class reference exposed to cinematics
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cinematic3DWidget Setup Essentials", meta=(DisplayName = "Widget Class"))
	TSubclassOf<UUserWidget> WidgetClass;

	// World or screen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cinematic3DWidget Setup Essentials", meta=(DisplayName = "Widget Render Space"))
	EWidgetSpace  WidgetSpace = EWidgetSpace::Screen;

};
