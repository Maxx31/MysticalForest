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
	AddHUD(IngameHUDClass, false, false);
}


UUserWidget* AMysticalForestGameMode::AddHUD(TSubclassOf<class UUserWidget> WidgetToAdd, bool ShowMouseCursor, bool EnableCLickEvents)
{
	AMysticalForestCharacter* Character = Cast<AMysticalForestCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	UUserWidget* newWidget;

	if (WidgetToAdd != nullptr)
	{
		GLog->Log("Hud was added");
		Controller->bShowMouseCursor = ShowMouseCursor; 
		Controller->bEnableClickEvents = EnableCLickEvents;

		newWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToAdd);
		newWidget->AddToViewport();

		return newWidget;
	}
	return nullptr;
}
