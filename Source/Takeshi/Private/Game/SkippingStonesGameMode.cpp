// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Game/SkippingStonesGameMode.h"

#include "EngineUtils.h"

#include "Game/Courses/SkippingStones/SkippingStone.h"
#include "Player/TakeshiPlayerController.h"


void ASkippingStonesGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitializeSkippingStones();
}

void ASkippingStonesGameMode::InitializeSkippingStones()
{
	for (TActorIterator<ASkippingStone> It(GetWorld(), ASkippingStone::StaticClass()); It; ++It)
	{
		ASkippingStone* SkippingStone = *It;
		SkippingStones.Add(SkippingStone);
	}

	for (ASkippingStone* SkippingStone : SkippingStones)
	{
		SkippingStone->OnCharacterInteractionBegin.AddDynamic(this, &ASkippingStonesGameMode::CharacterInteractionWithSkippingStoneBegin);
		SkippingStone->OnCharacterInteractionEnd.AddDynamic(this, &ASkippingStonesGameMode::CharacterInteractionWithSkippingStoneEnd);

		SkippingStone->SetDetectionRadius(MaximumValidDistanceBetweenSkippingStones);
	}
}

void ASkippingStonesGameMode::CharacterInteractionWithSkippingStoneBegin()
{
	TakeshiPlayerController->SetMovementForSlipperSurface();
}

void ASkippingStonesGameMode::CharacterInteractionWithSkippingStoneEnd()
{
	TakeshiPlayerController->SetMovementForNormalSurface();
}
