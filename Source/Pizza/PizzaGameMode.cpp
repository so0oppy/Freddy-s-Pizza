// Copyright Epic Games, Inc. All Rights Reserved.

#include "PizzaGameMode.h"
#include "PizzaCharacter.h"
#include "UObject/ConstructorHelpers.h"

APizzaGameMode::APizzaGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
