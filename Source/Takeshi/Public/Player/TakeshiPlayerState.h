// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "GameFramework/PlayerState.h"
#include "CoreMinimal.h"

#include "TakeshiPlayerState.generated.h"


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLivesChangedSignature, int32, NewPlayerLives);


UCLASS()
class TAKESHI_API ATakeshiPlayerState : public APlayerState
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable)
	void InitialisePlayerLives(const int32 InPlayerLives);

	UFUNCTION(BlueprintCallable)
	void DecrementPlayerLives();

	UPROPERTY(BlueprintAssignable)
	FOnPlayerLivesChangedSignature OnPlayerLivesChanged;


protected:


private:

	UPROPERTY(VisibleAnywhere, Category = "Core")
	int32 PlayerLives = 0;

};
