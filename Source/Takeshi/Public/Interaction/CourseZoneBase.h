// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"

#include "CourseZoneBase.generated.h"


// Forward Declarations
class UBoxComponent;
class USceneComponent;
class UStaticMeshComponent;


UCLASS(Abstract)
class TAKESHI_API ACourseZoneBase : public AActor
{
	GENERATED_BODY()


public:	

	ACourseZoneBase();


protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionVolume;


private:


};
