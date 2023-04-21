// Copyright Epic Games, Inc. All Rights Reserved.

#include "MysticalForestGameMode.h"
#include "MysticalForestCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "Blueprint/UserWidget.h"

AMysticalForestGameMode::AMysticalForestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AMysticalForestGameMode::BeginPlay()
{
	Controller = GetWorld()->GetFirstPlayerController();

	AddHUD(IngameHUDClass, false, false, true);
	InventoryUserWidget = AddHUD(InventoryHUDClass, false, false, true);
	CloseInventoryWidget();
}


UUserWidget* AMysticalForestGameMode::AddHUD(TSubclassOf<class UUserWidget> WidgetToAdd, bool ShowMouseCursor, bool EnableCLickEvents, bool AddToViewport)
{
	AMysticalForestCharacter* Character = Cast<AMysticalForestCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	UUserWidget* newWidget;

	if (WidgetToAdd != nullptr)
	{
		GLog->Log("Hud was added");
		Controller->bShowMouseCursor = ShowMouseCursor; 
		Controller->bEnableClickEvents = EnableCLickEvents;

		newWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToAdd);

		if(AddToViewport)
		newWidget->AddToViewport();

		return newWidget;
	}
	return nullptr;
}

void AMysticalForestGameMode::OpenInventoryWidget()
{
	GLog->Log("Open inventory");

	if(InventoryUserWidget != nullptr)
	{
		InventoryUserWidget->SetVisibility(ESlateVisibility::Visible);
		Controller->bShowMouseCursor = true;
		Controller->bEnableClickEvents = true;

		IsInventoryOpened = true;
	}
	else
	{
		GLog->Log("Error! Inventory widget does not exist");
	}
}

void AMysticalForestGameMode::CloseInventoryWidget()
{
	GLog->Log("Close inventory");

	if (InventoryUserWidget != nullptr)
	{
		InventoryUserWidget->SetVisibility(ESlateVisibility::Hidden);
		Controller->bShowMouseCursor = false;
		Controller->bEnableClickEvents = false;

		IsInventoryOpened = false;
	}
	else
	{
		GLog->Log("Error! Inventory widget does not exist");
	}
}
