// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "UI/GameOverUserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"


void UGameOverUserWidget::SetOutcomeText(const EGameOverOutcome Outcome) const
{
	if (OutcomeText)
	{
		switch (Outcome)
		{
		case EGameOverOutcome::PlayerWin:

			OutcomeText->SetText(FText::FromString("You Win!"));
			break;

		case EGameOverOutcome::PlayerLoss:

			OutcomeText->SetText(FText::FromString("You Lose!"));
			break;

		default:
			OutcomeText->SetText(FText::FromString("Undetermined"));
			break;
		}
	}
}

void UGameOverUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindDelegates();
}

void UGameOverUserWidget::BindDelegates()
{
	if (PlayAgainButton)
	{
		PlayAgainButton->OnClicked.AddDynamic(this, &UGameOverUserWidget::PlayAgainButtonClicked);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UGameOverUserWidget::MainMenuButtonClicked);
	}
}

void UGameOverUserWidget::PlayAgainButtonClicked()
{
	OnPlayAgainButtonClicked.Broadcast();
}

void UGameOverUserWidget::MainMenuButtonClicked()
{
	OnMainMenuButtonClicked.Broadcast();
}
