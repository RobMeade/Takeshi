// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Game/SkippingStonesGameMode.h"

#include "Player/TakeshiPlayerController.h"


void ASkippingStonesGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ASkippingStonesGameMode::PlayerControllerInitialized()
{
	Super::PlayerControllerInitialized();

	TakeshiPlayerController->InitializePlayerLives(InitialPlayerLives);
}

void ASkippingStonesGameMode::PlayerCharacterDestroyed()
{
	Super::PlayerCharacterDestroyed();

	if (!bIsCourseCompleted)
	{
		TakeshiPlayerController->DecrementPlayerLives();
	}
}

void ASkippingStonesGameMode::PlayerLivesChanged(int32 NewPlayerLives)
{
	Super::PlayerLivesChanged(NewPlayerLives);

	if (NewPlayerLives == 0)
	{
		TakeshiPlayerController->GameOver(EGameOverOutcome::PlayerLoss);
	}
	else if (NewPlayerLives > 0 && NewPlayerLives < InitialPlayerLives)
	{
		RestartPlayer(TakeshiPlayerController);	
	}
}

void ASkippingStonesGameMode::PlayerEnteredCourseEndZone()
{
	Super::PlayerEnteredCourseEndZone();
}
