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

bool ACourseGameModeBase::CanEditChange(const FProperty* InProperty) const
{
	bool bParentCanEditChange = Super::CanEditChange(InProperty);

	if (InProperty->GetName() == GET_MEMBER_NAME_CHECKED(ACourseGameModeBase, CourseTimerFunction))
	{
		return bParentCanEditChange && bEnableCourseTimer;
	}

	if (InProperty->GetName() == GET_MEMBER_NAME_CHECKED(ACourseGameModeBase, CourseDuration))
	{
		return bParentCanEditChange && bEnableCourseTimer && CourseTimerFunction == ECourseTimerFunction::Decrement;
	}

	if (InProperty->GetName() == GET_MEMBER_NAME_CHECKED(ACourseGameModeBase, MaximumCourseDuration))
	{
		return bParentCanEditChange && bEnableCourseTimer && CourseTimerFunction == ECourseTimerFunction::Increment;
	}

	return bParentCanEditChange;
}

void ACourseGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetCourseZones();
	BindDelegates();
}

void ACourseGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(CourseTimerHandle);
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

	CourseStartZone->OnPlayerExitedStartZone.AddDynamic(this, &ACourseGameModeBase::PlayerExitedCourseStartZone);
	CourseEndZone->OnPlayerEnteredCourseEndZone.AddDynamic(this, &ACourseGameModeBase::PlayerEnteredCourseEndZone);
}

void ACourseGameModeBase::PlayerControllerHasBegunPlay()
{
	TakeshiPlayerController->InitializeForGame();
}

void ACourseGameModeBase::PlayerControllerInitializationForGameCompleted()
{
	Super::PlayerControllerInitializationForGameCompleted();

	InitializeCourseTimer();
	InitializePlayerLives();
}

void ACourseGameModeBase::InitializeCourseTimer()
{
	if (bEnableCourseTimer)
	{
		if (CourseTimerFunction == ECourseTimerFunction::Decrement)
		{
			CourseTime = CourseDuration;
		}
		else if (CourseTimerFunction == ECourseTimerFunction::Increment)
		{
			CourseTime = 0;
		}

		TakeshiPlayerController->InitializeCourseTimer(CourseTime);
	}
}

void ACourseGameModeBase::InitializePlayerLives()
{
	if (bEnablePlayerLives)
	{
		TakeshiPlayerController->InitializePlayerLives(InitialPlayerLives);
	}
}

void ACourseGameModeBase::StartCourseTimer()
{
	if (bEnableCourseTimer)
	{
		GetWorld()->GetTimerManager().SetTimer(CourseTimerHandle, this, &ACourseGameModeBase::UpdateCourseTimer, 1.0f, true);
	}
}

void ACourseGameModeBase::StopCourseTimer()
{
	if (bEnableCourseTimer)
	{
		GetWorld()->GetTimerManager().ClearTimer(CourseTimerHandle);
	}
}

void ACourseGameModeBase::UpdateCourseTimer()
{
	if (CourseTimerFunction == ECourseTimerFunction::Decrement)
	{
		DecrementCourseTimer();
	}
	else if (CourseTimerFunction == ECourseTimerFunction::Increment)
	{
		IncrementCourseTimer();
	}
}

void ACourseGameModeBase::DecrementCourseTimer()
{
	CourseTime--;

	OnCourseTimeChanged.Broadcast(CourseTime);

	if (CourseTime == 0)
	{
		bIsGameOver = true;
		TakeshiPlayerController->GameOver(EGameOverOutcome::PlayerLoss);
	}
}

void ACourseGameModeBase::IncrementCourseTimer()
{
	CourseTime++;

	OnCourseTimeChanged.Broadcast(CourseTime);

	if (CourseTime == MaximumCourseDuration)
	{
		bIsGameOver = true;
		TakeshiPlayerController->GameOver(EGameOverOutcome::PlayerLoss);
	}
}

void ACourseGameModeBase::PlayerCharacterDestroyed()
{
	if (bEnablePlayerLives)
	{
		if (!bIsGameOver)
		{
			TakeshiPlayerController->DecrementPlayerLives();			
		}
	}
	else
	{
		TakeshiPlayerController->GameOver(EGameOverOutcome::PlayerLoss);
	}
}

void ACourseGameModeBase::PlayerLivesChanged(int32 NewPlayerLives)
{
	if (NewPlayerLives == 0)
	{
		bIsGameOver = true;
		TakeshiPlayerController->GameOver(EGameOverOutcome::PlayerLoss);
	}
	else if (NewPlayerLives > 0 && NewPlayerLives < InitialPlayerLives)
	{
		RestartPlayer(TakeshiPlayerController);
	}
}

void ACourseGameModeBase::PlayerEnteredCourseEndZone()
{
	StopCourseTimer();

	bIsGameOver = true;
	TakeshiPlayerController->GameOver(EGameOverOutcome::PlayerWin);
}

void ACourseGameModeBase::PlayerExitedCourseStartZone()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(CourseTimerHandle))
	{
		StartCourseTimer();
	}
}
