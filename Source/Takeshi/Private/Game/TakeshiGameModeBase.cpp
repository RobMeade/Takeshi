// Copyright Rob Meade & Ethan Mitchell.  All Rights Reserved.

#include "Game/TakeshiGameModeBase.h"

#include "UObject/ConstructorHelpers.h"

#include "Character/TakeshiCharacterBase.h"


ATakeshiGameModeBase::ATakeshiGameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Assets/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
