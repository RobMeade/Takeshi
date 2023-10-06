// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Interaction/CourseZoneBase.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"


ACourseZoneBase::ACourseZoneBase()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	CollisionVolume = CreateDefaultSubobject<UBoxComponent>("CollisionVolume");
	CollisionVolume->SetupAttachment(DefaultSceneRoot);
	CollisionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionVolume->SetBoxExtent(FVector(500.f, 300.f, 400.0f));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(DefaultSceneRoot);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACourseZoneBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ACourseZoneBase::OverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &ACourseZoneBase::OverlapEnd);
}

void ACourseZoneBase::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Note: Deliberately left empty, overriden in derived classes
}

void ACourseZoneBase::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Note: Deliberately left empty, overriden in derived classes
}
