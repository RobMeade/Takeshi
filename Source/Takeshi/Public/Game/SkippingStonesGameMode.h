// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "Game/TakeshiGameModeBase.h"
#include "CoreMinimal.h"

#include "SkippingStonesGameMode.generated.h"


// Forward Declarations
class ATakeshiPlayerController;


UCLASS()
class TAKESHI_API ASkippingStonesGameMode : public ATakeshiGameModeBase
{
	GENERATED_BODY()


public:

	
protected:

	virtual void BeginPlay() override;
	virtual void ReactToHazard() override;


private:

	void BindDelegates();

	UFUNCTION()
	void PlayerControllerInitialized();

	UFUNCTION()
	void PlayerLivesChanged(int32 NewPlayerLives);

	UPROPERTY()
	ATakeshiPlayerController* TakeshiPlayerController = nullptr;
	
};
