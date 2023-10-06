// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Interaction/CourseZoneBase.h"

#include "CourseStartZone.generated.h"


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerExitedStartZoneSignature);


UCLASS()
class TAKESHI_API ACourseStartZone : public ACourseZoneBase
{
	GENERATED_BODY()


public:

	FOnPlayerExitedStartZoneSignature OnPlayerExitedStartZone;


protected:

	virtual void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


private:


};
