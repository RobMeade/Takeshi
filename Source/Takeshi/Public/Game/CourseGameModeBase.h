// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Game/TakeshiGameModeBase.h"

#include "CourseGameModeBase.generated.h"


// Forward Declarations
class ACourseStartZone;
class ACourseEndZone;


UCLASS()
class TAKESHI_API ACourseGameModeBase : public ATakeshiGameModeBase
{
	GENERATED_BODY()


public:

	ACourseGameModeBase();


protected:

	virtual void BeginPlay() override;
	virtual void BindDelegates() override;
	virtual void PlayerControllerInitialized() override;

	UFUNCTION()
	virtual void ReactToHazard();

	UFUNCTION()
	virtual void PlayerLivesChanged(int32 NewPlayerLives);

	UFUNCTION()
	virtual void PlayerEnteredCourseEndZone();

	UFUNCTION()
	virtual void PlayerExitedCourseStartZone();

	UPROPERTY()
	TObjectPtr<ACourseStartZone> CourseStartZone = nullptr;

	UPROPERTY()
	TObjectPtr<ACourseEndZone> CourseEndZone = nullptr;

	UPROPERTY(EditAnywhere, Category = "Course Properties")
	int32 InitialPlayerLives = 0;


private:

	void SetCourseZones();

	
};
