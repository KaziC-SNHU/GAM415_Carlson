// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAM415_CarlsonGameMode.h"
#include "GAM415_CarlsonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAM415_CarlsonGameMode::AGAM415_CarlsonGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
