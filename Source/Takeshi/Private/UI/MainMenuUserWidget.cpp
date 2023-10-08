// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "UI/MainMenuUserWidget.h"

#include "Components/Button.h"


void UMainMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindDelegates();
}

void UMainMenuUserWidget::BindDelegates()
{
	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::PlayButtonClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::QuitButtonClicked);
	}
}

void UMainMenuUserWidget::PlayButtonClicked()
{
	OnPlayButtonClicked.Broadcast();
}

void UMainMenuUserWidget::QuitButtonClicked()
{
	OnQuitButtonClicked.Broadcast();
}
