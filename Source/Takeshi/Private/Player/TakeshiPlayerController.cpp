// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Player/TakeshiPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

#include "Character/TakeshiCharacterBase.h"
#include "Game/CourseGameModeBase.h"
#include "Player/TakeshiPlayerState.h"
#include "UI/GameOverUserWidget.h"
#include "UI/HUDUserWidget.h"
#include "UI/MainMenuUserWidget.h"


ATakeshiPlayerController::ATakeshiPlayerController()
{
	bReplicates = true;
}

void ATakeshiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetupPlayerState();

	OnHasBegunPlay.Broadcast();
}

void ATakeshiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATakeshiPlayerController::Move);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATakeshiPlayerController::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATakeshiPlayerController::StopJumping);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATakeshiPlayerController::Look);
}

void ATakeshiPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetupPlayerCharacter();
}

void ATakeshiPlayerController::SetupPlayerState()
{
	TakeshiPlayerState = GetPlayerState<ATakeshiPlayerState>();

	if (TakeshiPlayerState)
	{
		TakeshiPlayerState->OnPlayerLivesChanged.AddDynamic(this, &ATakeshiPlayerController::PlayerLivesChanged);
	}
}

void ATakeshiPlayerController::SetupPlayerCharacter()
{
	TakeshiCharacter = GetPawn<ATakeshiCharacterBase>();

	if (TakeshiCharacter)
	{
		TakeshiCharacter->OnDestroyed.AddDynamic(this, &ATakeshiPlayerController::PlayerCharacterDestroyed);
	}
}

//	Player Movement

void ATakeshiPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATakeshiPlayerController::Jump(const FInputActionValue& InputActionValue)
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->Jump();
	}
}

void ATakeshiPlayerController::StopJumping(const FInputActionValue& InputActionValue)
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->StopJumping();
	}
}

void ATakeshiPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATakeshiPlayerController::SetMovementForSlipperSurface() const
{
	if (TakeshiCharacter && TakeshiCharacter->GetCharacterMovement())
	{
		TakeshiCharacter->GetCharacterMovement()->BrakingDecelerationWalking = TakeshiCharacter->GetSlipperyBrakingDecelerationWalking();
		TakeshiCharacter->GetCharacterMovement()->GroundFriction = TakeshiCharacter->GetSlipperyGroundFriction();
	}
}

void ATakeshiPlayerController::SetMovementForNormalSurface() const
{
	if (TakeshiCharacter && TakeshiCharacter->GetCharacterMovement())
	{
		TakeshiCharacter->GetCharacterMovement()->BrakingDecelerationWalking = TakeshiCharacter->GetDefaultBrakingDecelerationWalking();
		TakeshiCharacter->GetCharacterMovement()->GroundFriction = TakeshiCharacter->GetDefaultGroundFriction();
	}
}

// Course Timer

void ATakeshiPlayerController::InitializeCourseTimer(const int32 InTimeInSeconds)
{
	CourseTimerChanged(InTimeInSeconds);
}

void ATakeshiPlayerController::CourseTimerChanged(int32 NewTimeInSeconds)
{
	if (HUDUserWidget)
	{
		HUDUserWidget->SetCourseTimer(NewTimeInSeconds);
	}
}

//	Player Lives

void ATakeshiPlayerController::InitializePlayerLives(const int32 InPlayerLives)
{
	TakeshiPlayerState->InitialisePlayerLives(InPlayerLives);
}

void ATakeshiPlayerController::DecrementPlayerLives() const
{
	TakeshiPlayerState->DecrementPlayerLives();
}

void ATakeshiPlayerController::PlayerLivesChanged(int32 NewPlayerLives)
{
	if (HUDUserWidget)
	{
		HUDUserWidget->SetPlayerLives(NewPlayerLives);
	}

	OnPlayerLivesChanged.Broadcast(NewPlayerLives);
}

void ATakeshiPlayerController::PlayerCharacterDestroyed(AActor* Actor)
{
	OnPlayerCharacterDestroyed.Broadcast();
}

//	Initialization

void ATakeshiPlayerController::InitializeForMainMenu()
{
	check(MainMenuUserWidgetClass);

	SetupMainMenuUIWidgets();
	SetInputModeUI();
}

void ATakeshiPlayerController::SetInputModeUI()
{
	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);
}

void ATakeshiPlayerController::InitializeForGame()
{
	check(TakeshiCharacter);
	check(TakeshiPlayerState);
	check(TakeshiContext);
	check(HUDUserWidgetClass);
	check(GameOverUserWidgetClass);

	SetupCourseGameMode();
	SetupGameOverUIWidgets();
	SetupHUDUIWidgets();
	AddInputMappingContext();
	SetInputModeGame();

	OnInitializationForGameCompleted.Broadcast();
}

void ATakeshiPlayerController::AddInputMappingContext()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(TakeshiContext, 0);
	}
}

void ATakeshiPlayerController::SetInputModeGame()
{
	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
}

//	Game Over

void ATakeshiPlayerController::GameOver(const EGameOverOutcome Outcome)
{
	if (!GameOverUserWidget->IsInViewport())
	{
		DisableInput(this);
		SetInputModeUI();

		if (GameOverUserWidget)
		{
			GameOverUserWidget->SetOutcomeText(Outcome);
			GameOverUserWidget->AddToViewport();		
		}		
	}
}

//	User-Interfaces

void ATakeshiPlayerController::SetupMainMenuUIWidgets()
{
	MainMenuUserWidget = CreateWidget<UMainMenuUserWidget>(this, MainMenuUserWidgetClass);

	if (MainMenuUserWidget)
	{
		MainMenuUserWidget->OnPlayButtonClicked.AddDynamic(this, &ATakeshiPlayerController::MainMenuPlayButtonClicked);
		MainMenuUserWidget->OnQuitButtonClicked.AddDynamic(this, &ATakeshiPlayerController::MainMenuQuitButtonClicked);

		MainMenuUserWidget->AddToViewport();
	}
}

void ATakeshiPlayerController::SetupGameOverUIWidgets()
{
	GameOverUserWidget = CreateWidget<UGameOverUserWidget>(this, GameOverUserWidgetClass);

	if (GameOverUserWidget)
	{
		GameOverUserWidget->OnPlayAgainButtonClicked.AddDynamic(this, &ATakeshiPlayerController::GameOverPlayAgainButtonClicked);
		GameOverUserWidget->OnMainMenuButtonClicked.AddDynamic(this, &ATakeshiPlayerController::GameOverMainMenuButtonClicked);
	}
}

void ATakeshiPlayerController::SetupHUDUIWidgets()
{
	HUDUserWidget = CreateWidget<UHUDUserWidget>(this, HUDUserWidgetClass);

	HUDUserWidget->AddToViewport();
}

void ATakeshiPlayerController::SetupCourseGameMode()
{
	CourseGameMode = Cast<ACourseGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (CourseGameMode)
	{
		CourseGameMode->OnCourseTimeChanged.AddDynamic(this, &ATakeshiPlayerController::CourseTimerChanged);
	}
}

void ATakeshiPlayerController::MainMenuPlayButtonClicked()
{
	OnMainMenuPlayButtonClicked.Broadcast();
}

void ATakeshiPlayerController::MainMenuQuitButtonClicked()
{
	OnMainMenuQuitButtonClicked.Broadcast();
}

void ATakeshiPlayerController::GameOverPlayAgainButtonClicked()
{
	OnGameOverPlayAgainButtonClicked.Broadcast();
}

void ATakeshiPlayerController::GameOverMainMenuButtonClicked()
{
	OnGameOverMainMenuButtonClicked.Broadcast();
}
