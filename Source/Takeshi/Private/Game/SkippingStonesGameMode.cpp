// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Game/SkippingStonesGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "Player/TakeshiPlayerController.h"


void ASkippingStonesGameMode::BeginPlay()
{
	Super::BeginPlay();

	// NOTE: Make assumption of single player game with PlayerIndex 0
	TakeshiPlayerController = CastChecked<ATakeshiPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	BindDelegates();
}

void ASkippingStonesGameMode::BindDelegates()
{
	TakeshiPlayerController->OnInitialized.AddDynamic(this, &ASkippingStonesGameMode::PlayerControllerInitialized);
	TakeshiPlayerController->OnPlayerLivesChanged.AddDynamic(this, &ASkippingStonesGameMode::PlayerLivesChanged);
}

void ASkippingStonesGameMode::PlayerControllerInitialized()
{
	// TODO: Set lives to be a configuration variable for the GameMode
	TakeshiPlayerController->InitialisePlayerLives(3);
}

void ASkippingStonesGameMode::ReactToHazard()
{
	Super::ReactToHazard();

	TakeshiPlayerController->DecrementPlayerLives();
}

void ASkippingStonesGameMode::PlayerLivesChanged(int32 NewPlayerLives)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Lives Remaining: %d"), NewPlayerLives);

	if (NewPlayerLives == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over - You Lose"));
	}
}
