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

	TakeshiPlayerController->OnMainMenuPlayButtonClicked.AddDynamic(this, &ATakeshiGameModeBase::PlayGame);
	TakeshiPlayerController->OnMainMenuQuitButtonClicked.AddDynamic(this, &ATakeshiGameModeBase::QuitGame);
	TakeshiPlayerController->OnGameOverPlayAgainButtonClicked.AddDynamic(this, &ATakeshiGameModeBase::PlayAgain);
	TakeshiPlayerController->OnGameOverMainMenuButtonClicked.AddDynamic(this, &ATakeshiGameModeBase::ReturnToMainMenu);
}

void ATakeshiGameModeBase::PlayerControllerInitialized()
{
	// Note: Deliberately left empty, overriden in derived classes
}

void ATakeshiGameModeBase::PlayGame()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, FirstCourseMap);
}

void ATakeshiGameModeBase::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, TakeshiPlayerController, EQuitPreference::Quit, true);
}

void ATakeshiGameModeBase::PlayAgain()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, FirstCourseMap);
}

void ATakeshiGameModeBase::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainMenuMap);
}
