// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CoreMinimal.h"

#include "TakeshiPlayerController.generated.h"


// Forward Declarations
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerReactToHazardSignature);


UCLASS()
class TAKESHI_API ATakeshiPlayerController : public APlayerController
{
	GENERATED_BODY()


public:

	ATakeshiPlayerController();

	UPROPERTY(BlueprintAssignable)
	FOnControllerReactToHazardSignature OnControllerReactToHazard;


protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;


private:

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> TakeshiContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	void Move(const FInputActionValue& InputActionValue);
	void Jump(const FInputActionValue& InputActionValue);
	void StopJumping(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void ReactToHazard();
};
