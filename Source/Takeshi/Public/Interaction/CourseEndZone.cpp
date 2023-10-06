// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "CourseEndZone.h"

#include "Character/TakeshiCharacterBase.h"


void ACourseEndZone::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ATakeshiCharacterBase* TakeshiPlayerCharacter = Cast<ATakeshiCharacterBase>(OtherActor);

	if (TakeshiPlayerCharacter)
	{
		OnPlayerEnteredCourseEndZone.Broadcast();		
	}
}

void ACourseEndZone::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	// ...
}
