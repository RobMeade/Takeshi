// Copyright Rob Meade & Ethan Mitchell.  All Rights Reserved.

#include "Game/TakeshiGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "Player/TakeshiPlayerController.h"


ATakeshiGameModeBase::ATakeshiGameModeBase()
{
	// ...
}

void ATakeshiGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// NOTE: Making assumption of single player game with PlayerIndex 0
	TakeshiPlayerController = CastChecked<ATakeshiPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
}

void ATakeshiGameModeBase::BindDelegates()
{
	TakeshiPlayerController->OnHasBegunPlay.AddDynamic(this, &ATakeshiGameModeBase::PlayerControllerInitialized);
}

void ATakeshiGameModeBase::PlayerControllerInitialized()
{
	// Note: Deliberately left empty, overriden in derived classes
}
