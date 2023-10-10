// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "UI/HUDUserWidget.h"

#include "UI/PlayerLivesUserWidget.h"


void UHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHUDUserWidget::SetPlayerLives(const int32 NewPlayerLives) const
{
	if (PlayerLives)
	{
		PlayerLives->SetPlayerLives(NewPlayerLives);
	}
}
