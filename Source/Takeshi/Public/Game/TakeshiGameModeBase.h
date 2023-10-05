// Copyright Rob Meade & Ethan Mitchell.  All Rights Reserved.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "CoreMinimal.h"

#include "TakeshiGameModeBase.generated.h"


UCLASS(minimalapi, Abstract)
class ATakeshiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:

	ATakeshiGameModeBase();


protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ReactToHazard();


private:


};
