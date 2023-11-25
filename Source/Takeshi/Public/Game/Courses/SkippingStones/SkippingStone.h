// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"

#include "Game/Courses/SkippingStones/SkippingStoneResponsivity.h"
#include "Interaction/CharacterInteractionInterface.h"

#include "SkippingStone.generated.h"


// Forward Declarations
class UPhysicsConstraintComponent;
class USphereComponent;
class UStaticMeshComponent;

class ATakeshiCharacterBase;


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkippingStoneCharacterInteractionBeginSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkippingStoneCharacterInteractionEndSignature);


UCLASS()
class TAKESHI_API ASkippingStone : public AActor, public ICharacterInteractionInterface
{
	GENERATED_BODY()


public:	

	ASkippingStone();

	FOnSkippingStoneCharacterInteractionBeginSignature OnCharacterInteractionBegin;
	FOnSkippingStoneCharacterInteractionEndSignature OnCharacterInteractionEnd;

	ESkippingStoneResponsivity GetResponsivity() { return Responsivity; }

	virtual void CharacterInteractionBegin(ATakeshiCharacterBase* InteractingCharacter) override;
	virtual void CharacterInteractionEnd(ATakeshiCharacterBase* InteractingCharacter) override;
	void Initialize();
	void SetDetectionRadius(float Radius);

#if WITH_EDITOR

	virtual void PostEditMove(bool bFinished) override;
	virtual void PostEditUndo() override;
	virtual void PostEditUndo(TSharedPtr<ITransactionObjectAnnotation> TransactionAnnotation) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif


protected:

	virtual void BeginPlay() override;

	// TODO: Change this to "VisibleAnywhere", once the gamemode is responsible for setting which stones are responsive
	UPROPERTY(EditAnywhere, Category = "Takeshi|SkippingStone")
	ESkippingStoneResponsivity Responsivity = ESkippingStoneResponsivity::NotSet;

	UPROPERTY(EditAnywhere, Category = "Takeshi|SkippingStone|Movement")
	float DefaultLinearDrivePositionStrength = 1.f;

	UPROPERTY(EditAnywhere, Category = "Takeshi|SkippingStone|Movement")
	float DefaultLinearDriveVelocityStrength = 1.f;;

	UPROPERTY(EditAnywhere, Category = "Takeshi|SkippingStone|Movement")
	float DefaultLinearDriveForceLimit = 0.f;;

	UPROPERTY(EditAnywhere, Category = "Takeshi|SkippingStone|Movement")
	float RestitutiveLinearDrivePositionStrength = 100.f;

	UPROPERTY(EditAnywhere, Category = "Takeshi|SkippingStone|Movement")
	float RestitutiveLinearDriveVelocityStrength = 25.f;

	UPROPERTY(EditAnywhere, Category = "Takeshi|SkippingStone|Movement")
	float RestitutiveLinearDriveForceLimit = 0.f;


private:

#if WITH_EDITOR

	void CheckPathValidity();
	void CheckStoneValidityForPath();

	void InitializeForPathFinding();
	void GetZones();

	bool FindPath(AActor* Start, AActor* Goal, TArray<AActor*>& OutPath);
	AActor* FindLowestCostNode(TArray<AActor*>& OpenSet, AActor* Goal);
	float CalculateTotalCost(AActor* Node, AActor* Goal);
	float CalculateHeuristic(AActor* Node, AActor* Goal);
	TArray<AActor*> ReconstructPath(AActor* Goal);
	AActor* GetParent(AActor* Node);
	TArray<AActor*> GetNeighbors(AActor* CurrentNode);
	TArray<AActor*> GetNearbyActors(AActor* CurrentNode);
	bool IsValidNeighbor(AActor* CurrentNode, AActor* Neighbor);
	float CalculateTentativeCost(AActor* CurrentNode, AActor* Neighbor);
	float CalculateCostToMove(AActor* NodeA, AActor* NodeB);
	void InitializeCosts(AActor* Start);
	float GetCost(AActor* Node);
	void SetCost(AActor* Node, float NewCost);
	void InitializeParents(AActor* Start);
	void SetParent(AActor* Node, AActor* Parent);

#endif

	UPROPERTY(EditDefaultsOnly, Category = "Takeshi|SkippingStone|Components")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, Category = "Takeshi|SkippingStone|Components")
	TObjectPtr<USphereComponent> DetectionVolume;

	UPROPERTY(EditDefaultsOnly, Category = "Takeshi|SkippingStone|Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Takeshi|SkippingStone|Components")
	TObjectPtr<UPhysicsConstraintComponent> PhysicsConstraint;

	UPROPERTY(EditDefaultsOnly, Category = "Takeshi|SkippingStone|Components")
	TObjectPtr<UBillboardComponent> DetectionBillboard;

	UPROPERTY(EditDefaultsOnly, Category = "Takeshi|SkippingStone|Pathfinding Validation")
	TObjectPtr<UTexture2D> DetectionInvalidTexture;

	UPROPERTY()
	float DetectionRadius = 200.f;

#if WITH_EDITORONLY_DATA

	UPROPERTY(VisibleAnywhere, Category = "Takeshi")
	TMap<AActor*, float> PathingNodeCostMap;

	UPROPERTY(VisibleAnywhere, Category = "Takeshi")
	TMap<AActor*, AActor*> PathingNodeParentMap;

	UPROPERTY()
	TObjectPtr<AActor> CourseStartZone;

	UPROPERTY()
	TObjectPtr<AActor> CourseEndZone;

#endif

};
