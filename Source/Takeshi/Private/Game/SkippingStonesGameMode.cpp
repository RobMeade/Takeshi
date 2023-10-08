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

void ASkippingStonesGameMode::ReactToHazard()
{
	Super::ReactToHazard();

	TakeshiPlayerController->DecrementPlayerLives();
}

void ASkippingStonesGameMode::PlayerLivesChanged(int32 NewPlayerLives)
{
	Super::PlayerLivesChanged(NewPlayerLives);

	if (NewPlayerLives > 0 && NewPlayerLives < InitialPlayerLives)
	{
		UE_LOG(LogTemp, Warning, TEXT("Respawning Player Character"));
		RestartPlayer(TakeshiPlayerController);
	}

	UE_LOG(LogTemp, Warning, TEXT("Player Lives Remaining: %d"), NewPlayerLives);

	if (NewPlayerLives == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over - Player Loses"));
	}
}

void ASkippingStonesGameMode::PlayerEnteredCourseEndZone()
{
	Super::PlayerEnteredCourseEndZone();
}
