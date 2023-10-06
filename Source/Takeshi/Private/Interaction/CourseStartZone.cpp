// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Interaction/CourseStartZone.h"

#include "Character/TakeshiCharacterBase.h"


void ACourseStartZone::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// ...
}

void ACourseStartZone::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	ATakeshiCharacterBase* TakeshiPlayerCharacter = Cast<ATakeshiCharacterBase>(OtherActor);

	if (TakeshiPlayerCharacter)
	{
		OnPlayerExitedStartZone.Broadcast();
	}
}
