// Copyright Epic Games, Inc. All Rights Reserved.

#include "MysticalForestGameMode.h"
#include "MysticalForestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMysticalForestGameMode::AMysticalForestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
