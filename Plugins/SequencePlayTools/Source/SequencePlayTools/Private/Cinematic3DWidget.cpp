// Fill out your copyright notice in the Description page of Project Settings.


#include "Cinematic3DWidget.h"


//find text widgets includes start
#include <functional>
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "Components/PanelWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
//find text widgets includes end

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

TArray<UWidget*> ACinematic3DWidget::GetAllTextWidgets()
{
	TArray<UWidget*> Result;

	if (!WidgetComponent) return Result;

	UUserWidget* RootWidget = WidgetComponent->GetUserWidgetObject();
	if (!RootWidget) return Result;

	// Recursive lambda to find all widgets able to display text
	std::function<void(UPanelWidget*)> FindTextWidgets;
	FindTextWidgets = [&Result, &FindTextWidgets](UPanelWidget* PanelWidget)
	{
		if (!PanelWidget) return;

		for (int32 i = 0; i < PanelWidget->GetChildrenCount(); ++i)
		{
			UWidget* Child = PanelWidget->GetChildAt(i);
			if (UTextBlock* TextBlock = Cast<UTextBlock>(Child))
			{
				Result.Add(TextBlock);
			}
			else if (UEditableText* EditableText = Cast<UEditableText>(Child))
			{
				Result.Add(EditableText);
			}
			else if (UEditableTextBox* EditableTextBox = Cast<UEditableTextBox>(Child))
			{
				Result.Add(EditableTextBox);
			}
			else if (URichTextBlock* RichTextBlock = Cast<URichTextBlock>(Child))
			{
				Result.Add(RichTextBlock);
			}
			else if (UPanelWidget* ChildPanel = Cast<UPanelWidget>(Child))
			{
				FindTextWidgets(ChildPanel); // Recurse into child panels
			}
		}
	};

	// Start recursion with the root widget
	if (UPanelWidget* RootPanel = Cast<UPanelWidget>(RootWidget->GetRootWidget()))
	{
		FindTextWidgets(RootPanel);
	}

	return Result;
}

