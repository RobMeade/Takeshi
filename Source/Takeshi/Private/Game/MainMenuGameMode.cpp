// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Game/MainMenuGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Player/TakeshiPlayerController.h"


void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	BindDelegates();
}

void AMainMenuGameMode::BindDelegates()
{
	Super::BindDelegates();

	TakeshiPlayerController->OnMainMenuPlayButtonClicked.AddDynamic(this, &AMainMenuGameMode::PlayGame);
	TakeshiPlayerController->OnMainMenuQuitButtonClicked.AddDynamic(this, &AMainMenuGameMode::QuitGame);
}

void AMainMenuGameMode::PlayerControllerInitialized()
{
	Super::PlayerControllerInitialized();

	DisableInput(TakeshiPlayerController);
	TakeshiPlayerController->InitializeForMainMenu();
}

void AMainMenuGameMode::PlayGame()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, FirstCourseMap);
}

void AMainMenuGameMode::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, TakeshiPlayerController, EQuitPreference::Quit, true);
}
