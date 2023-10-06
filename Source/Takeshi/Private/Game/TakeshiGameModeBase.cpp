// Copyright Rob Meade & Ethan Mitchell.  All Rights Reserved.

#include "Game/TakeshiGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "Interaction/CourseStartZone.h"
#include "Interaction/CourseEndZone.h"
#include "Player/TakeshiPlayerController.h"


ATakeshiGameModeBase::ATakeshiGameModeBase()
{
	// ...
}

void ATakeshiGameModeBase::BeginPlay()
{
	// NOTE: Making assumption of single player game with PlayerIndex 0
	TakeshiPlayerController = CastChecked<ATakeshiPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	SetCourseZones();
	BindDelegates();
}

void ATakeshiGameModeBase::SetCourseZones()
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

void ATakeshiGameModeBase::BindDelegates()
{
	TakeshiPlayerController->OnReactToHazard.AddDynamic(this, &ATakeshiGameModeBase::ReactToHazard);
	TakeshiPlayerController->OnInitialized.AddDynamic(this, &ATakeshiGameModeBase::PlayerControllerInitialized);
	TakeshiPlayerController->OnPlayerLivesChanged.AddDynamic(this, &ATakeshiGameModeBase::PlayerLivesChanged);

	CourseStartZone->OnPlayerExitedStartZone.AddDynamic(this, &ATakeshiGameModeBase::PlayerExitedCourseStartZone);
	CourseEndZone->OnPlayerEnteredCourseEndZone.AddDynamic(this, &ATakeshiGameModeBase::PlayerEnteredCourseEndZone);
}

void ATakeshiGameModeBase::PlayerControllerInitialized()
{
	// Note: Deliberately left empty, overriden in derived classes
}

void ATakeshiGameModeBase::ReactToHazard()
{
	// Note: Deliberately left empty, overriden in derived classes
}

void ATakeshiGameModeBase::PlayerLivesChanged(int32 NewPlayerLives)
{
	// Note: Deliberately left empty, overriden in derived classes
}

void ATakeshiGameModeBase::PlayerEnteredCourseEndZone()
{
	// Note: Deliberately left empty, overriden in derived classes
	UE_LOG(LogTemp, Warning, TEXT("Game Over - Player Wins"));
}

void ATakeshiGameModeBase::PlayerExitedCourseStartZone()
{
	// Note: Deliberately left empty, overriden in derived classes
	UE_LOG(LogTemp, Warning, TEXT("Player exited Start Zone"));
}
