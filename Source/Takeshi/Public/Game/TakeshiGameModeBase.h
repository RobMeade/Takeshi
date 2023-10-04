// Copyright Rob Meade & Ethan Mitchell.  All Rights Reserved.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "CoreMinimal.h"

#include "TakeshiGameModeBase.generated.h"


UCLASS(minimalapi)
class ATakeshiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:

	ATakeshiGameModeBase();


protected:

	virtual void BeginPlay() override;


private:

	UFUNCTION()
	void ReactToHazard();

};
