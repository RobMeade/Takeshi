// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Interaction/Hazard.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Interaction/ReactToHazardInterface.h"


AHazard::AHazard()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	CollisionVolume = CreateDefaultSubobject<UBoxComponent>("CollisionVolume");
	CollisionVolume->SetupAttachment(DefaultSceneRoot);
	CollisionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionVolume->SetBoxExtent(FVector(100.f, 100.f, 25.0f));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(DefaultSceneRoot);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHazard::BeginPlay()
{
	Super::BeginPlay();

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AHazard::OverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AHazard::OverlapEnd);
}

void AHazard::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IReactToHazardInterface* ReactToHazardInterface = Cast<IReactToHazardInterface>(OtherActor))
	{
		ReactToHazardInterface->ReactToHazard();
	}
}

void AHazard::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// ...
}
