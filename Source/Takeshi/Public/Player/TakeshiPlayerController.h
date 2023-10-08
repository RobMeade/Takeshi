// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CoreMinimal.h"
#include "UI/MainMenuUserWidget.h"

#include "TakeshiPlayerController.generated.h"


// Forward Declarations
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

class ATakeshiCharacterBase;
class ATakeshiPlayerState;


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerHasBegunPlaySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerMainMenuPlayButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerMainMenuQuitButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerReactToHazardSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerPlayerLivesChangedSignature, int32, NewPlayerLives);


UCLASS()
class TAKESHI_API ATakeshiPlayerController : public APlayerController
{
	GENERATED_BODY()


public:

	ATakeshiPlayerController();

	UPROPERTY()
	FOnControllerHasBegunPlaySignature OnHasBegunPlay;

	UPROPERTY(BlueprintAssignable)
	FOnControllerReactToHazardSignature OnReactToHazard;

	UPROPERTY(BlueprintAssignable)
	FOnControllerPlayerLivesChangedSignature OnPlayerLivesChanged;

	UPROPERTY(BlueprintAssignable)
	FOnControllerMainMenuPlayButtonClickedSignature OnMainMenuPlayButtonClicked;

	UPROPERTY(BlueprintAssignable)
	FOnControllerMainMenuQuitButtonClickedSignature OnMainMenuQuitButtonClicked;

	void InitializeForMainMenu();
	void InitializeForGame();

	void InitializePlayerLives(const int32 InPlayerLives);
	void DecrementPlayerLives() const;


protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "User Interfaces")
	TSubclassOf<UUserWidget> MainMenuUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UMainMenuUserWidget> MainMenuUserWidget = nullptr;


private:

	UFUNCTION()
	void MainMenuPlayButtonClicked();

	UFUNCTION()
	void MainMenuQuitButtonClicked();

	void CreateUIWidgets();

	void BindUIDelegates();
	void BindDelegates();

	void SetInputModeUI();
	void SetInputModeGame();

	void AddInputMappingContext();

	void Move(const FInputActionValue& InputActionValue);
	void Jump(const FInputActionValue& InputActionValue);
	void StopJumping(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void PlayerLivesChanged(int32 NewPlayerLives);

	UFUNCTION()
	void ReactToHazard();

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> TakeshiContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction = nullptr;

	UPROPERTY()
	TObjectPtr<ATakeshiPlayerState> TakeshiPlayerState = nullptr;

	UPROPERTY()
	TObjectPtr<ATakeshiCharacterBase> TakeshiCharacter = nullptr;

};
