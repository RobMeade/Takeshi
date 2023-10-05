// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Game/SkippingStonesGameMode.h"


void ASkippingStonesGameMode::ReactToHazard()
{
	Super::ReactToHazard();

	UE_LOG(LogTemp, Warning, TEXT("Player is losing a life"));
}
