// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CoreMinimal.h"

#include "TakeshiPlayerController.generated.h"


// Forward Declarations
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

class ATakeshiPlayerState;


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerInitializedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerReactToHazardSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerPlayerLivesChangedSignature, int32, NewPlayerLives);


UCLASS()
class TAKESHI_API ATakeshiPlayerController : public APlayerController
{
	GENERATED_BODY()


public:

	ATakeshiPlayerController();

	UPROPERTY()
	FOnControllerInitializedSignature OnInitialized;

	UPROPERTY(BlueprintAssignable)
	FOnControllerReactToHazardSignature OnReactToHazard;

	UPROPERTY(BlueprintAssignable)
	FOnControllerPlayerLivesChangedSignature OnPlayerLivesChanged;

	void InitialisePlayerLives(const int32 InPlayerLives);
	void DecrementPlayerLives() const;


protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;


private:

	void Move(const FInputActionValue& InputActionValue);
	void Jump(const FInputActionValue& InputActionValue);
	void StopJumping(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void PlayerLivesChanged(int32 NewPlayerLives);

	UFUNCTION()
	void ReactToHazard();

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> TakeshiContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY()
	ATakeshiPlayerState* TakeshiPlayerState = nullptr;

};
