// Fill out your copyright notice in the Description page of Project Settings.


#include "Cinematic3DWidget.h"

// Sets default values
ACinematic3DWidget::ACinematic3DWidget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	SetRootComponent(WidgetComponent);
}

// Called when the game starts or when spawned
void ACinematic3DWidget::BeginPlay()
{
	Super::BeginPlay();
	
	WidgetComponent->SetWidgetSpace(WidgetSpace);

	if (!IsValid(WidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("ACinematic3DWidget: Invalid Widget Class! Please set it up in the Category All->Cinematic3DWidget Setup Essentials"));
	}
	
}

// Called every frame
void ACinematic3DWidget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

