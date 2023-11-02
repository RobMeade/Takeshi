// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Game/CourseGameModeBase.h"

#include "SkippingStonesGameMode.generated.h"


// Forward Declarations
class ASkippingStone;


UCLASS()
class TAKESHI_API ASkippingStonesGameMode : public ACourseGameModeBase
{
	GENERATED_BODY()


public:

	
protected:

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "SkippingStones")
	float MaximumValidDistanceBetweenSkippingStones = 100.f;


private:

	void InitializeSkippingStones();

	UFUNCTION()
	void CharacterInteractionWithSkippingStoneBegin();

	UFUNCTION()
	void CharacterInteractionWithSkippingStoneEnd();

	UPROPERTY()
	TArray<ASkippingStone*> SkippingStones;
	
};
