// Copyright Epic Games, Inc. All Rights Reserved.

#include "TakeshiGameMode.h"
#include "TakeshiCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATakeshiGameMode::ATakeshiGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
