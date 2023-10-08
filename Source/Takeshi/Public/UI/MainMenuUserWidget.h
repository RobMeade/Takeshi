// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "MainMenuUserWidget.generated.h"


// Forward Declarations
class UButton;


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMainMenuUserWidgetPlayButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMainMenuUserWidgetQuitButtonClickedSignature);


UCLASS()
class TAKESHI_API UMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	FOnMainMenuUserWidgetPlayButtonClickedSignature OnPlayButtonClicked;
	FOnMainMenuUserWidgetQuitButtonClickedSignature OnQuitButtonClicked;


protected:

	virtual void NativeConstruct() override;


private:

	UFUNCTION()
	void PlayButtonClicked();

	UFUNCTION()
	void QuitButtonClicked();

	void BindDelegates();

	UPROPERTY(meta = (BindWidget))
	UButton* PlayButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton = nullptr;
	
};
