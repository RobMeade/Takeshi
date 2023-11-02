// Copyright Rob Meade & Ethan Mitchell.  All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "CoreMinimal.h"

#include "Interaction/ReactToHazardInterface.h"

#include "TakeshiCharacterBase.generated.h"


// Forward Declarations
class UCameraComponent;
class USpringArmComponent;


UCLASS(config=Game)
class ATakeshiCharacterBase : public ACharacter, public IReactToHazardInterface
{
	GENERATED_BODY()


public:

	ATakeshiCharacterBase();

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE float GetDefaultBrakingDecelerationWalking() const { return DefaultBrakingDecelerationWalking; }
	FORCEINLINE float GetDefaultGroundFriction() const { return DefaultGroundFriction; }
	FORCEINLINE float GetSlipperyBrakingDecelerationWalking() const { return SlipperyBrakingDecelerationWalking; }
	FORCEINLINE float GetSlipperyGroundFriction() const { return SlipperyGroundFriction; }

	virtual void Tick(float DeltaSeconds) override;
	virtual void ReactToHazard() override;


protected:

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, Category = "Takeshi|Character|Movement")
	float SlipperyBrakingDecelerationWalking = 350.f;

	UPROPERTY(EditAnywhere, Category = "Takeshi|Character|Movement")
	float SlipperyGroundFriction = 0.f;


private:

	void CheckFloorForCharacterInteraction();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY()
	float DefaultBrakingDecelerationWalking;

	UPROPERTY()
	float DefaultGroundFriction;

	UPROPERTY()
	AActor* LastFloorHitActor = nullptr;

};
