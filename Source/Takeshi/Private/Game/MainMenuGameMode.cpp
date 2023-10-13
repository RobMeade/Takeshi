// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Game/MainMenuGameMode.h"

#include "Player/TakeshiPlayerController.h"


void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	BindDelegates();
}

void AMainMenuGameMode::BindDelegates()
{
	Super::BindDelegates();
}

void AMainMenuGameMode::PlayerControllerHasBegunPlay()
{
	Super::PlayerControllerHasBegunPlay();

	DisableInput(TakeshiPlayerController);
	TakeshiPlayerController->InitializeForMainMenu();
}
