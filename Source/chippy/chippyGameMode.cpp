// Copyright Epic Games, Inc. All Rights Reserved.

#include "chippyGameMode.h"
#include "chippyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AchippyGameMode::AchippyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
