// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Game/Courses/SkippingStones/SkippingStone.h"

#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "Character/TakeshiCharacterBase.h"


ASkippingStone::ASkippingStone()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	DetectionVolume = CreateDefaultSubobject<USphereComponent>("DetectionVolume");
	DetectionVolume->SetupAttachment(DefaultSceneRoot);
	DetectionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionVolume->SetSphereRadius(DetectionRadius);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(DefaultSceneRoot);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(false);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>("PhysicsConstraint");
	PhysicsConstraint->SetupAttachment(DefaultSceneRoot);

	PhysicsConstraint->SetRelativeLocation(FVector(0.f, 0.f, -60.f));
	PhysicsConstraint->SetConstrainedComponents(Mesh, EName::None, nullptr, EName::None);
	PhysicsConstraint->SetLinearXLimit(LCM_Locked, 0.f);
	PhysicsConstraint->SetLinearYLimit(LCM_Locked, 0.f);
	PhysicsConstraint->SetLinearZLimit(LCM_Limited, 50.f);
	PhysicsConstraint->ConstraintInstance.ProfileInstance.LinearLimit.bSoftConstraint = true;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.LinearLimit.Stiffness = 5.f;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.LinearLimit.Damping = 0.f;
	PhysicsConstraint->SetAngularSwing1Limit(ACM_Limited, 45.f);
	PhysicsConstraint->SetAngularSwing2Limit(ACM_Limited, 45.f);
	PhysicsConstraint->SetAngularTwistLimit(ACM_Limited, 45.f);
	PhysicsConstraint->ConstraintInstance.ProfileInstance.ConeLimit.bSoftConstraint = true;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.ConeLimit.Stiffness = 50.f;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.ConeLimit.Damping = 5.f;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.TwistLimit.bSoftConstraint = true;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.TwistLimit.Stiffness = 50.f;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.TwistLimit.Damping = 5.f;
	PhysicsConstraint->SetLinearPositionDrive(false, false, true);
	PhysicsConstraint->SetLinearPositionTarget(FVector::ZeroVector);
	PhysicsConstraint->SetLinearVelocityDrive(false, false, true);
	PhysicsConstraint->SetLinearVelocityTarget(FVector::ZeroVector);
	PhysicsConstraint->SetLinearDriveParams(1.f, 1.f, 0.f);
	PhysicsConstraint->SetAngularDriveMode(EAngularDriveMode::SLERP);
	PhysicsConstraint->SetAngularOrientationTarget(FRotator::ZeroRotator);
	PhysicsConstraint->SetOrientationDriveSLERP(true);
	PhysicsConstraint->SetAngularVelocityTarget(FVector::ZeroVector);
	PhysicsConstraint->SetAngularVelocityDriveSLERP(true);
	PhysicsConstraint->SetAngularDriveParams(10.f, 1.f, 0.f);
}

void ASkippingStone::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void ASkippingStone::Initialize()
{
	if (Responsivity != ESkippingStoneResponsivity::Responsive)
	{
		if (Mesh)
		{
			Mesh->SetSimulatePhysics(false);
		}
	}
	else
	{
		if (PhysicsConstraint)
		{
			PhysicsConstraint->ConstraintInstance.GetLinearDriveParams(DefaultLinearDrivePositionStrength, DefaultLinearDriveVelocityStrength, DefaultLinearDriveForceLimit);
		}		
	}
}

void ASkippingStone::SetDetectionRadius(float Radius)
{
	DetectionRadius = Radius;

	if (DetectionVolume)
	{
		DetectionVolume->SetSphereRadius(DetectionRadius);
	}
}

void ASkippingStone::CharacterInteractionBegin(ATakeshiCharacterBase* InteractingCharacter)
{
	if (PhysicsConstraint)
	{
		PhysicsConstraint->SetLinearDriveParams(DefaultLinearDrivePositionStrength, DefaultLinearDriveVelocityStrength, DefaultLinearDriveForceLimit);
	}

	OnCharacterInteractionBegin.Broadcast();
}

void ASkippingStone::CharacterInteractionEnd(ATakeshiCharacterBase* InteractingCharacter)
{
	if (PhysicsConstraint)
	{
		PhysicsConstraint->SetLinearDriveParams(RestitutiveLinearDrivePositionStrength, RestitutiveLinearDriveVelocityStrength, RestitutiveLinearDriveForceLimit);
	}

	OnCharacterInteractionEnd.Broadcast();
}
