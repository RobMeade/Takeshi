// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "CourseTimerFunction.h"
#include "TakeshiGameModeBase.h"

#include "CourseGameModeBase.generated.h"


// Forward Declarations
class ACourseStartZone;
class ACourseEndZone;


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCourseGameModeCourseTimeChangedSignature, int32, NewTimeInSeconds);


UCLASS()
class TAKESHI_API ACourseGameModeBase : public ATakeshiGameModeBase
{
	GENERATED_BODY()


public:

	ACourseGameModeBase();

	FOnCourseGameModeCourseTimeChangedSignature OnCourseTimeChanged;

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BindDelegates() override;
	virtual void PlayerControllerHasBegunPlay() override;
	virtual void PlayerControllerInitializationForGameCompleted() override;
	virtual void InitializeCourseTimer();
	virtual void InitializePlayerLives();
	virtual void StartCourseTimer();
	virtual void StopCourseTimer();

	UFUNCTION()
	virtual void PlayerCharacterDestroyed();

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

	UPROPERTY(EditAnywhere, Category = "Course|Player")
	bool bEnablePlayerLives = true;

	UPROPERTY(EditAnywhere, Category = "Course|Player", meta = (EditCondition = "bEnablePlayerLives"))
	int32 InitialPlayerLives = 3;

	UPROPERTY(EditAnywhere, Category = "Course|Timer")
	bool bEnableCourseTimer = false;

	UPROPERTY(EditAnywhere, Category = "Course|Timer")
	ECourseTimerFunction CourseTimerFunction = ECourseTimerFunction::Decrement;

	UPROPERTY(EditAnywhere, Category = "Course|Timer", meta = (EditCondition = "CourseTimerFunction == ECourseTimerFunction::Decrement"))
	int32 CourseDuration = 60;

	UPROPERTY(EditAnywhere, Category = "Course|Timer", meta = (EditCondition = "CourseTimerFunction == ECourseTimerFunction::Increment"))
	int32 MaximumCourseDuration = 60;

	FTimerHandle CourseTimerHandle;

	bool bIsGameOver = false;


private:

	void SetCourseZones();
	void UpdateCourseTimer();
	void DecrementCourseTimer();
	void IncrementCourseTimer();

	int32 CourseTime = 0;
	
};
