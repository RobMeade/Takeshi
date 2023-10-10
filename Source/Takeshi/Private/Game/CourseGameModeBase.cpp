// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Game/CourseGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "Game/GameOverOutcome.h"
#include "Interaction/CourseStartZone.h"
#include "Interaction/CourseEndZone.h"
#include "Player/TakeshiPlayerController.h"


ACourseGameModeBase::ACourseGameModeBase()
{
	// ...
}

void ACourseGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetCourseZones();
	BindDelegates();
}

void ACourseGameModeBase::SetCourseZones()
{
	TArray<AActor*> FoundCourseStartZones;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACourseStartZone::StaticClass(), FoundCourseStartZones);

	TArray<AActor*> FoundCourseEndZones;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACourseEndZone::StaticClass(), FoundCourseEndZones);

	ensureMsgf(FoundCourseStartZones.Num() == 1, TEXT("The course must have 1, and only 1 Start Zone.  This course contains %d Start Zone(s)"), FoundCourseStartZones.Num());
	ensureMsgf(FoundCourseEndZones.Num() == 1, TEXT("The course must have 1, and only 1 End Zone.  This course contains %d End Zone(s)"), FoundCourseEndZones.Num());

	CourseStartZone = Cast<ACourseStartZone>(FoundCourseStartZones[0]);
	CourseEndZone = Cast<ACourseEndZone>(FoundCourseEndZones[0]);
}

void ACourseGameModeBase::BindDelegates()
{
	Super::BindDelegates();

	TakeshiPlayerController->OnPlayerCharacterDestroyed.AddDynamic(this, &ACourseGameModeBase::PlayerCharacterDestroyed);
	TakeshiPlayerController->OnPlayerLivesChanged.AddDynamic(this, &ACourseGameModeBase::PlayerLivesChanged);

	TakeshiPlayerController->OnGameOverPlayAgainButtonClicked.AddDynamic(this, &ACourseGameModeBase::PlayAgain);
	TakeshiPlayerController->OnGameOverMainMenuButtonClicked.AddDynamic(this, &ACourseGameModeBase::ReturnToMainMenu);

	CourseStartZone->OnPlayerExitedStartZone.AddDynamic(this, &ACourseGameModeBase::PlayerExitedCourseStartZone);
	CourseEndZone->OnPlayerEnteredCourseEndZone.AddDynamic(this, &ACourseGameModeBase::PlayerEnteredCourseEndZone);
}

void ACourseGameModeBase::PlayerControllerInitialized()
{
	TakeshiPlayerController->InitializeForGame();
}

void ACourseGameModeBase::PlayerCharacterDestroyed()
{
	// Note: Deliberately left empty, overriden in derived classes
}

void ACourseGameModeBase::PlayerLivesChanged(int32 NewPlayerLives)
{
	// Note: Deliberately left empty, overriden in derived classes
}

void ACourseGameModeBase::PlayerEnteredCourseEndZone()
{
	bIsCourseCompleted = true;
	TakeshiPlayerController->GameOver(EGameOverOutcome::PlayerWin);
}

void ACourseGameModeBase::PlayerExitedCourseStartZone()
{
	// Note: Deliberately left empty, overriden in derived classes
	UE_LOG(LogTemp, Warning, TEXT("Player exited Start Zone"));
}

void ACourseGameModeBase::PlayAgain()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, FirstCourseMap);
}

void ACourseGameModeBase::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainMenuMap);
}
