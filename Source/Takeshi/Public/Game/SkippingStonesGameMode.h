// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "Game/CourseGameModeBase.h"
#include "CoreMinimal.h"

#include "SkippingStonesGameMode.generated.h"


UCLASS()
class TAKESHI_API ASkippingStonesGameMode : public ACourseGameModeBase
{
	GENERATED_BODY()


public:

	
protected:

	virtual void BeginPlay() override;
	virtual void PlayerControllerInitialized() override;
	virtual void ReactToHazard() override;
	virtual void PlayerLivesChanged(int32 NewPlayerLives) override;
	virtual void PlayerEnteredCourseEndZone() override;


private:

	
};
