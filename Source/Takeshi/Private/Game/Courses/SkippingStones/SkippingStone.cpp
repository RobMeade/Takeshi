// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Game/Courses/SkippingStones/SkippingStone.h"

#include "Components/BillboardComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PropertyEditorModule.h"

#include "Character/TakeshiCharacterBase.h"


ASkippingStone::ASkippingStone()
{
	PrimaryActorTick.bCanEverTick = false;

	/*	Custom Property Section */
	static const FName PropertyEditor("PropertyEditor");
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);

#define LOCTEXT_NAMESPACE "PropertySection"

	TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection("Object", "Takeshi", LOCTEXT("Takeshi", "Takeshi"));
	Section->AddCategory("Takeshi");

#undef LOCTEXT_NAMESPACE
	/*	End Custom Property Section */

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

#if WITH_EDITOR

	DetectionBillboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("DetectionBillboard");
	DetectionBillboard->SetupAttachment(DefaultSceneRoot);
	DetectionBillboard->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	DetectionBillboard->SetUsingAbsoluteScale(true);
	DetectionBillboard->SetVisibility(false);
	DetectionBillboard->bHiddenInGame = true;
	DetectionBillboard->bIsScreenSizeScaled = true;

	if (DetectionInvalidTexture)
	{
		DetectionBillboard->Sprite = DetectionInvalidTexture;
	}

#endif
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

/*	Skipping Stones Pathfinding Validity */

#if WITH_EDITOR
void ASkippingStone::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (bFinished)
	{
		CheckPathValidity();
	}
}

void ASkippingStone::PostEditUndo()
{
	Super::PostEditUndo();

	CheckPathValidity();
}

void ASkippingStone::PostEditUndo(TSharedPtr<ITransactionObjectAnnotation> TransactionAnnotation)
{
	Super::PostEditUndo(TransactionAnnotation);

	CheckPathValidity();
}

void ASkippingStone::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CheckPathValidity();
}

void ASkippingStone::CheckPathValidity()
{
	CheckStoneValidityForPath();
}

void ASkippingStone::CheckStoneValidityForPath()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray<AActor*> FoundActors;
	const bool bDetectedSkippingStones = UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), DetectionVolume->GetScaledSphereRadius(), ObjectTypes, ASkippingStone::StaticClass(), ActorsToIgnore, FoundActors);

	if (DetectionBillboard)
	{
		DetectionBillboard->SetVisibility(!bDetectedSkippingStones);
	}
}
#endif
