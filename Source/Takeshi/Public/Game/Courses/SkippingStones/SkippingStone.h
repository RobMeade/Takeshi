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


protected:

	virtual void BeginPlay() override;

	// TODO: Change this to "VisibleAnywhere", once the gamemode is responsible for setting which stones are responsive
	UPROPERTY(EditAnywhere, Category = "SkippingStone")
	ESkippingStoneResponsivity Responsivity = ESkippingStoneResponsivity::NotSet;

	UPROPERTY(EditAnywhere, Category = "SkippingStone|Movement")
	float DefaultLinearDrivePositionStrength = 1.f;

	UPROPERTY(EditAnywhere, Category = "SkippingStone|Movement")
	float DefaultLinearDriveVelocityStrength = 1.f;;

	UPROPERTY(EditAnywhere, Category = "SkippingStone|Movement")
	float DefaultLinearDriveForceLimit = 0.f;;

	UPROPERTY(EditAnywhere, Category = "SkippingStone|Movement")
	float RestitutiveLinearDrivePositionStrength = 100.f;

	UPROPERTY(EditAnywhere, Category = "SkippingStone|Movement")
	float RestitutiveLinearDriveVelocityStrength = 25.f;

	UPROPERTY(EditAnywhere, Category = "SkippingStone|Movement")
	float RestitutiveLinearDriveForceLimit = 0.f;


private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> DetectionVolume;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsConstraintComponent> PhysicsConstraint;

	UPROPERTY()
	float DetectionRadius = 100.f;

};
