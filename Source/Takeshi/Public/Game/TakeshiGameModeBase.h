// Copyright Rob Meade & Ethan Mitchell.  All Rights Reserved.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "CoreMinimal.h"

#include "TakeshiGameModeBase.generated.h"


// Forward Declarations
class ATakeshiPlayerController;


UCLASS(minimalapi, Abstract)
class ATakeshiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:

	ATakeshiGameModeBase();


protected:

	virtual void BeginPlay() override;
	virtual void BindDelegates();

	UFUNCTION()
	virtual void PlayerControllerInitialized();

	UPROPERTY()
	TObjectPtr<ATakeshiPlayerController> TakeshiPlayerController = nullptr;

	UPROPERTY(EditAnywhere, Category = "Maps")
	TSoftObjectPtr<UWorld> MainMenuMap = nullptr;

	UPROPERTY(EditAnywhere, Category = "Maps")
	TSoftObjectPtr<UWorld> FirstCourseMap = nullptr;


private:

	UFUNCTION()
	void PlayGame();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void PlayAgain();

	UFUNCTION()
	void ReturnToMainMenu();

};
