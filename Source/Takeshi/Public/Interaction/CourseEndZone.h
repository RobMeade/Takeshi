// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Interaction/CourseZoneBase.h"

#include "CourseEndZone.generated.h"


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnterredCourseEndZoneSignature);


UCLASS()
class TAKESHI_API ACourseEndZone : public ACourseZoneBase
{
	GENERATED_BODY()


public:

	FOnPlayerEnterredCourseEndZoneSignature OnPlayerEnteredCourseEndZone;


protected:

	virtual void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


private:

		
};
