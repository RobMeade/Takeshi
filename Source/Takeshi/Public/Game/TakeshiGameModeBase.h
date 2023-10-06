// Copyright Rob Meade & Ethan Mitchell.  All Rights Reserved.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "CoreMinimal.h"

#include "TakeshiGameModeBase.generated.h"


// Forward Declarations
class ACourseStartZone;
class ACourseEndZone;
class ATakeshiPlayerController;


UCLASS(minimalapi, Abstract)
class ATakeshiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:

	ATakeshiGameModeBase();


protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void PlayerControllerInitialized();

	UFUNCTION()
	virtual void ReactToHazard();

	UFUNCTION()
	virtual void PlayerLivesChanged(int32 NewPlayerLives);

	UFUNCTION()
	virtual void PlayerEnteredCourseEndZone();

	UFUNCTION()
	virtual void PlayerExitedCourseStartZone();

	UPROPERTY()
	TObjectPtr<ATakeshiPlayerController> TakeshiPlayerController = nullptr;

	UPROPERTY()
	TObjectPtr<ACourseStartZone> CourseStartZone = nullptr;

	UPROPERTY()
	TObjectPtr<ACourseEndZone> CourseEndZone = nullptr;


private:

	void SetCourseZones();
	void BindDelegates();

};
