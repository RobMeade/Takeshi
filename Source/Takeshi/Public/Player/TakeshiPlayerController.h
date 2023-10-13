// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CoreMinimal.h"

#include "Game/GameOverOutcome.h"

#include "TakeshiPlayerController.generated.h"


// Forward Declarations
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

class ACourseGameModeBase;
class ATakeshiCharacterBase;
class ATakeshiPlayerState;
class UGameOverUserWidget;
class UHUDUserWidget;
class UMainMenuUserWidget;


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerHasBegunPlaySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerInitializationForGameCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerInitializationForMainMenuCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerMainMenuPlayButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerMainMenuQuitButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerGameOverPlayAgainButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerGameOverMainMenuButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerPlayerCharacterDestroyedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerPlayerLivesChangedSignature, int32, NewPlayerLives);


UCLASS()
class TAKESHI_API ATakeshiPlayerController : public APlayerController
{
	GENERATED_BODY()


public:

	ATakeshiPlayerController();

	UPROPERTY()
	FOnControllerHasBegunPlaySignature OnHasBegunPlay;

	UPROPERTY()
	FOnControllerInitializationForGameCompletedSignature OnInitializationForGameCompleted;

	UPROPERTY()
	FOnControllerInitializationForMainMenuCompletedSignature OnInitializationForMainMenuCompleted;

	UPROPERTY(BlueprintAssignable)
	FOnControllerPlayerCharacterDestroyedSignature OnPlayerCharacterDestroyed;

	UPROPERTY(BlueprintAssignable)
	FOnControllerPlayerLivesChangedSignature OnPlayerLivesChanged;

	UPROPERTY(BlueprintAssignable)
	FOnControllerMainMenuPlayButtonClickedSignature OnMainMenuPlayButtonClicked;

	UPROPERTY(BlueprintAssignable)
	FOnControllerMainMenuQuitButtonClickedSignature OnMainMenuQuitButtonClicked;

	UPROPERTY(BlueprintAssignable)
	FOnControllerGameOverPlayAgainButtonClickedSignature OnGameOverPlayAgainButtonClicked;

	UPROPERTY(BlueprintAssignable)
	FOnControllerGameOverMainMenuButtonClickedSignature OnGameOverMainMenuButtonClicked;

	void InitializeForMainMenu();
	void InitializeForGame();

	void InitializeCourseTimer(const int32 InTimeInSeconds);
	void InitializePlayerLives(const int32 InPlayerLives);
	void DecrementPlayerLives() const;

	void GameOver(const EGameOverOutcome Outcome);


protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, Category = "User Interfaces")
	TSubclassOf<UUserWidget> MainMenuUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UMainMenuUserWidget> MainMenuUserWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "User Interfaces")
	TSubclassOf<UUserWidget> GameOverUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UGameOverUserWidget> GameOverUserWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "User Interfaces")
	TSubclassOf<UUserWidget> HUDUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UHUDUserWidget> HUDUserWidget = nullptr;


private:

	UFUNCTION()
	void MainMenuPlayButtonClicked();

	UFUNCTION()
	void MainMenuQuitButtonClicked();

	UFUNCTION()
	void GameOverPlayAgainButtonClicked();

	UFUNCTION()
	void GameOverMainMenuButtonClicked();

	void SetupCourseGameMode();
	void SetupPlayerState();
	void SetupPlayerCharacter();
	void SetupMainMenuUIWidgets();
	void SetupGameOverUIWidgets();
	void SetupHUDUIWidgets();

	void SetInputModeUI();
	void SetInputModeGame();

	void AddInputMappingContext();

	void Move(const FInputActionValue& InputActionValue);
	void Jump(const FInputActionValue& InputActionValue);
	void StopJumping(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void CourseTimerChanged(int32 NewTimeInSeconds);

	UFUNCTION()
	void PlayerLivesChanged(int32 NewPlayerLives);

	UFUNCTION()
	void PlayerCharacterDestroyed(AActor* Actor);

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> TakeshiContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction = nullptr;

	UPROPERTY()
	TObjectPtr<ACourseGameModeBase> CourseGameMode = nullptr;

	UPROPERTY()
	TObjectPtr<ATakeshiPlayerState> TakeshiPlayerState = nullptr;

	UPROPERTY()
	TObjectPtr<ATakeshiCharacterBase> TakeshiCharacter = nullptr;

};
