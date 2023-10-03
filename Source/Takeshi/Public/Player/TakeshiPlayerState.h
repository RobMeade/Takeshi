// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "GameFramework/PlayerState.h"
#include "CoreMinimal.h"

#include "TakeshiPlayerState.generated.h"


UCLASS()
class TAKESHI_API ATakeshiPlayerState : public APlayerState
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintPure)
	int32 GetPlayerLives() const;

	UFUNCTION(BlueprintCallable)
	void SetPlayerLives(const int32 Lives);


protected:


private:

	UPROPERTY(VisibleAnywhere, Category = "Core")
	int32 PlayerLives = 0;
	
};
