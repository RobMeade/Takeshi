// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Game/TakeshiGameModeBase.h"

#include "MainMenuGameMode.generated.h"


UCLASS()
class TAKESHI_API AMainMenuGameMode : public ATakeshiGameModeBase
{
	GENERATED_BODY()


public:


protected:

	virtual void BeginPlay() override;
	virtual void BindDelegates() override;
	virtual void PlayerControllerInitialized() override;

	UPROPERTY(EditAnywhere, Category = "Courses")
	TSoftObjectPtr<UWorld> FirstCourseMap = nullptr;


private:

	UFUNCTION()
	void PlayGame();

	UFUNCTION()
	void QuitGame();

};
