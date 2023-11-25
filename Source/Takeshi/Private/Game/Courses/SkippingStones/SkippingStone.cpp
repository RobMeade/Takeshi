// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Game/Courses/SkippingStones/SkippingStone.h"

#include "Components/BillboardComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "EngineUtils.h"
#include "PropertyEditorModule.h"

#include "Character/TakeshiCharacterBase.h"
#include "Interaction/CourseEndZone.h"
#include "Interaction/CourseStartZone.h"

// TODO: GameplayTags usage in this class needs to be improved

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

	GetZones();
	TArray<AActor*> Path;
	if (!FindPath(CourseStartZone, CourseEndZone, Path))
	{
		UE_LOG(LogTemp, Error, TEXT("The path from Start Zone to End Zone is not valid.  Check Skipping Stones are within range of each other and the Start and End Zones."));
		UE_LOG(LogTemp, Error, TEXT("There must be a path of UnResponsive Skipping Stones from the Start Zone to the End Zone."));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Path between Start Zone and End Zone is valid."));
	}
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

bool ASkippingStone::FindPath(AActor* Start, AActor* Goal, TArray<AActor*>& OutPath)
{
	// Initialise costs and parents maps
	InitializeForPathFinding();

	// Initialize open and closed sets
	TArray<AActor*> OpenSet;
	TArray<AActor*> ClosedSet;

	// Add the start node to the open set
	OpenSet.Add(Start);

	while (OpenSet.Num() > 0)
	{
		// Find the node with the lowest total cost in the open set
		AActor* CurrentNode = FindLowestCostNode(OpenSet, Goal);

		// Move CurrentNode from OpenSet to ClosedSet
		OpenSet.Remove(CurrentNode);
		ClosedSet.Add(CurrentNode);

		// If the goal node is reached, reconstruct and return the path
		if (CurrentNode == Goal)
		{
			OutPath = ReconstructPath(CurrentNode);
			return true;
		}

		// Process neighbors
		TArray<AActor*> Neighbors = GetNeighbors(CurrentNode);
		for (AActor* Neighbor : Neighbors)
		{
			if (ClosedSet.Contains(Neighbor))
			{
				continue;
			}

			const float TentativeCost = CalculateTentativeCost(CurrentNode, Neighbor);
			if (!OpenSet.Contains(Neighbor) || TentativeCost < GetCost(Neighbor))
			{
				SetCost(Neighbor, TentativeCost);
				SetParent(Neighbor, CurrentNode);

				if (!OpenSet.Contains(Neighbor))
				{
					OpenSet.Add(Neighbor);
				}
			}
		}
	}

	// If the loop completes without finding a path, return false
	return false;
}

void ASkippingStone::InitializeForPathFinding()
{
	InitializeParents(CourseStartZone);
	InitializeCosts(CourseStartZone);
}

void ASkippingStone::GetZones()
{
	TArray<AActor*> FoundCourseStartZones;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACourseStartZone::StaticClass(), FoundCourseStartZones);

	TArray<AActor*> FoundCourseEndZones;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACourseEndZone::StaticClass(), FoundCourseEndZones);

	ensureMsgf(FoundCourseStartZones.Num() == 1, TEXT("The course must have 1, and only 1 Start Zone.  This course contains %d Start Zone(s)"), FoundCourseStartZones.Num());
	ensureMsgf(FoundCourseEndZones.Num() == 1, TEXT("The course must have 1, and only 1 End Zone.  This course contains %d End Zone(s)"), FoundCourseEndZones.Num());

	CourseStartZone = FoundCourseStartZones[0];
	CourseEndZone = FoundCourseEndZones[0];

	CourseStartZone->Tags.AddUnique(FName("Takeshi.Zones.Start"));
	CourseEndZone->Tags.AddUnique(FName("Takeshi.Zones.End"));
}

AActor* ASkippingStone::FindLowestCostNode(TArray<AActor*>& OpenSet, AActor* Goal)
{
	AActor* LowestCostNode = nullptr;
	float LowestCost = MAX_FLT;

	for (AActor* Node : OpenSet)
	{
		const float TotalCost = CalculateTotalCost(Node, Goal);
		if (TotalCost < LowestCost)
		{
			LowestCost = TotalCost;
			LowestCostNode = Node;
		}
	}

	return LowestCostNode;
}

float ASkippingStone::CalculateTotalCost(AActor* Node, AActor* Goal)
{
	const float CostFromStart = GetCost(Node);
	const float EstimatedCostToGoal = CalculateHeuristic(Node, Goal);

	return CostFromStart + EstimatedCostToGoal;
}

float ASkippingStone::CalculateHeuristic(AActor* Node, AActor* Goal)
{
	const FVector StartLocation = Node->GetActorLocation();
	const FVector GoalLocation = Goal->GetActorLocation();

	return FVector::Distance(StartLocation, GoalLocation);
}

TArray<AActor*> ASkippingStone::ReconstructPath(AActor* Goal)
{
	TArray<AActor*> Path;

	// Start from the goal node and follow the parent pointers until reaching the start node
	AActor* CurrentNode = Goal;
	while (CurrentNode != nullptr)
	{
		// Insert at beginning to maintain the correct order 
		Path.Insert(CurrentNode, 0);
		CurrentNode = GetParent(CurrentNode);
	}

	return Path;
}

AActor* ASkippingStone::GetParent(AActor* Node)
{
	if (PathingNodeParentMap.Contains(Node))
	{
		return PathingNodeParentMap[Node];
	}

	// No parent found
	return nullptr;
}

TArray<AActor*> ASkippingStone::GetNeighbors(AActor* CurrentNode)
{
	TArray<AActor*> Neighbors;
	TArray<AActor*> NearbyActors = GetNearbyActors(CurrentNode);

	for (AActor* Neighbor : NearbyActors)
	{
		if (IsValidNeighbor(CurrentNode, Neighbor))
		{
			Neighbors.Add(Neighbor);
		}
	}

	return Neighbors;
}

TArray<AActor*> ASkippingStone::GetNearbyActors(AActor* CurrentNode)
{
	TArray<AActor*> NearbyActors;

	if (CurrentNode->Tags.Contains(FName("Takeshi.Zones.Start")))
	{
		for (TActorIterator<ASkippingStone> It(GetWorld(), ASkippingStone::StaticClass()); It; ++It)
		{
			ASkippingStone* SkippingStone = *It;

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(SkippingStone);

			TArray<AActor*> OverlappedActors;
			bool bCourseStartZoneOverlapped = UKismetSystemLibrary::SphereOverlapActors(this, SkippingStone->GetActorLocation(), SkippingStone->DetectionVolume->GetScaledSphereRadius(), ObjectTypes, ACourseStartZone::StaticClass(), ActorsToIgnore, OverlappedActors);

			if (bCourseStartZoneOverlapped)
			{
				NearbyActors.Add(SkippingStone);
			}
		}
	}
	else
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(CurrentNode);

		TArray<AActor*> OverlappedActors;
		UKismetSystemLibrary::SphereOverlapActors(this, CurrentNode->GetActorLocation(), DetectionVolume->GetScaledSphereRadius(), ObjectTypes, nullptr, ActorsToIgnore, OverlappedActors);

		for (AActor* OverlappedActor : OverlappedActors)
		{
			if (OverlappedActor->IsA(ASkippingStone::StaticClass()) || OverlappedActor->IsA(ACourseEndZone::StaticClass()))
			{
				NearbyActors.Add(OverlappedActor);
			}
		}
	}

	return NearbyActors;
}

bool ASkippingStone::IsValidNeighbor(AActor* CurrentNode, AActor* Neighbor)
{
	// We are only interested in either a SkippingStone or a CourseEndZone
	if (const ASkippingStone* Stone = Cast<ASkippingStone>(Neighbor))
	{
		if (Stone && Stone->Responsivity != ESkippingStoneResponsivity::Responsive)
		{
			return true;
		}		
	}
	else if (Neighbor->IsA(ACourseEndZone::StaticClass()))
	{
		return true;
	}

	return false;
}

float ASkippingStone::CalculateTentativeCost(AActor* CurrentNode, AActor* Neighbor)
{
	// Get the cost to move from one node to another
	const float CostToMove = CalculateCostToMove(CurrentNode, Neighbor);

	// Get the cost to reach the current node
	const float CostFromStart = GetCost(CurrentNode);

	// Calculate the tentative cost
	const float TentativeCost = CostFromStart + CostToMove;

	return TentativeCost;
}

float ASkippingStone::CalculateCostToMove(AActor* NodeA, AActor* NodeB)
{
	const FVector LocationA = NodeA->GetActorLocation();
	const FVector LocationB = NodeA->GetActorLocation();

	const float CostToMove = FVector::Distance(LocationA, LocationB);

	return CostToMove;
}

// Must be called before the A* algorithm executes
void ASkippingStone::InitializeCosts(AActor* Start)
{
	PathingNodeCostMap.Empty();
	PathingNodeCostMap.Add(Start, 0.f);
}

float ASkippingStone::GetCost(AActor* Node)
{
	if (PathingNodeCostMap.Contains(Node))
	{
		return PathingNodeCostMap[Node];
	}

	// If the node is not found in the map, return the default cost
	return MAX_FLT;
}

void ASkippingStone::SetCost(AActor* Node, float NewCost)
{
	PathingNodeCostMap.Add(Node, NewCost);
}

// Must be called before the A* algorithm executes
void ASkippingStone::InitializeParents(AActor* Start)
{
	PathingNodeParentMap.Empty();

	// The start node has no parent
	PathingNodeParentMap.Add(Start, nullptr);
}

void ASkippingStone::SetParent(AActor* Node, AActor* Parent)
{
	PathingNodeParentMap.Add(Node, Parent);
}

#endif
