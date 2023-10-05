// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "Game/TakeshiGameModeBase.h"
#include "CoreMinimal.h"

#include "SkippingStonesGameMode.generated.h"


UCLASS()
class TAKESHI_API ASkippingStonesGameMode : public ATakeshiGameModeBase
{
	GENERATED_BODY()


public:


protected:

	virtual void ReactToHazard() override;

	
private:
	
};
