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
	ATakeshiPlayerController* TakeshiPlayerController = Cast<ATakeshiPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (TakeshiPlayerController != nullptr)
	{
		TakeshiPlayerController->OnReactToHazard.AddDynamic(this, &ATakeshiGameModeBase::ReactToHazard);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("I dont have a player controller yet!"));
	}
}

void ATakeshiGameModeBase::ReactToHazard()
{
	// Note: Deliberate left empty, overriden in derived classes
}
