// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "UI/PlayerLivesUserWidget.h"

#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"


void UPlayerLivesUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerLivesUserWidget::SetPlayerLives(const int32 NewPlayerLives)
{
	if (PlayerLives)
	{
		PlayerLives->ClearChildren();

		if (NewPlayerLives > 0)
		{
			for (int32 i = 0; i < NewPlayerLives; i++)
			{
				if (PlayerLifeUserWidgetClass)
				{
					UUserWidget* PlayerLifeUserWidget = CreateWidget<UUserWidget>(this, PlayerLifeUserWidgetClass);
					UHorizontalBoxSlot* HorizontalBoxSlot = PlayerLives->AddChildToHorizontalBox(PlayerLifeUserWidget);
					HorizontalBoxSlot->SetPadding(FMargin(5.f));
					HorizontalBoxSlot->SetHorizontalAlignment(HAlign_Center);
					HorizontalBoxSlot->SetVerticalAlignment(VAlign_Center);
					HorizontalBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
				}
			}
		}
	}
}
