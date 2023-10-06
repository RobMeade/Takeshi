// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Player/TakeshiPlayerState.h"


void ATakeshiPlayerState::InitialisePlayerLives(const int32 InPlayerLives)
{
	PlayerLives = InPlayerLives;

	if (PlayerLives < 0) { PlayerLives = 0; }

	OnPlayerLivesChanged.Broadcast(PlayerLives);
}

void ATakeshiPlayerState::DecrementPlayerLives()
{
	PlayerLives--;

	if (PlayerLives < 0) { PlayerLives = 0; }

	OnPlayerLivesChanged.Broadcast(PlayerLives);
}
