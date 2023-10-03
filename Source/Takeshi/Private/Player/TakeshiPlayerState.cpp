// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Player/TakeshiPlayerState.h"


int32 ATakeshiPlayerState::GetPlayerLives() const
{
	return PlayerLives;
}

void ATakeshiPlayerState::SetPlayerLives(const int32 Lives)
{
	PlayerLives = Lives;
}
