// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "Game/GameOverOutcome.h"

#include "GameOverUserWidget.generated.h"


// Forward Declarations
class UButton;
class UTextBlock;


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverUserWidgetPlayAgainButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOveerUserWidgetMainMenuButtonClickedSignature);


UCLASS()
class TAKESHI_API UGameOverUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	FOnGameOverUserWidgetPlayAgainButtonClickedSignature OnPlayAgainButtonClicked;
	FOnGameOveerUserWidgetMainMenuButtonClickedSignature OnMainMenuButtonClicked;

	void SetOutcomeText(const EGameOverOutcome Outcome) const;


protected:

	virtual void NativeConstruct() override;


private:

	UFUNCTION()
	void PlayAgainButtonClicked();

	UFUNCTION()
	void MainMenuButtonClicked();

	void BindDelegates();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OutcomeText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* PlayAgainButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuButton = nullptr;
};
