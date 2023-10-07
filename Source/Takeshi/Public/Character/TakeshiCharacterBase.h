// Copyright Rob Meade & Ethan Mitchell.  All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "CoreMinimal.h"

#include "Interaction/ReactToHazardInterface.h"

#include "TakeshiCharacterBase.generated.h"


// Forward Declarations
class UCameraComponent;
class USpringArmComponent;


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterReactToHazardSignature);


UCLASS(config=Game)
class ATakeshiCharacterBase : public ACharacter, public IReactToHazardInterface
{
	GENERATED_BODY()


public:

	ATakeshiCharacterBase();

	UPROPERTY(BlueprintAssignable)
	FOnCharacterReactToHazardSignature OnReactToHazard;

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual void ReactToHazard() override;


protected:

	virtual void Destroyed() override;


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

};
