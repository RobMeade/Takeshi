// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Player/TakeshiPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Character/TakeshiCharacterBase.h"
#include "Player/TakeshiPlayerState.h"


ATakeshiPlayerController::ATakeshiPlayerController()
{
	bReplicates = true;
}

void ATakeshiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TakeshiCharacter = GetPawn<ATakeshiCharacterBase>();
	TakeshiPlayerState = GetPlayerState<ATakeshiPlayerState>();

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
	OnPlayerLivesChanged.Broadcast(NewPlayerLives);
}

void ATakeshiPlayerController::ReactToHazard()
{
	OnReactToHazard.Broadcast();
}

void ATakeshiPlayerController::InitializeForMainMenu()
{
	check(MainMenuUserWidgetClass);

	CreateUIWidgets();
	BindUIDelegates();
	SetInputModeUI();
}

void ATakeshiPlayerController::CreateUIWidgets()
{
	MainMenuUserWidget = CreateWidget<UMainMenuUserWidget>(this, MainMenuUserWidgetClass);
	MainMenuUserWidget->AddToViewport();
}

void ATakeshiPlayerController::BindUIDelegates()
{
	if (MainMenuUserWidget)
	{
		MainMenuUserWidget->OnPlayButtonClicked.AddDynamic(this, &ATakeshiPlayerController::MainMenuPlayButtonClicked);
		MainMenuUserWidget->OnQuitButtonClicked.AddDynamic(this, &ATakeshiPlayerController::MainMenuQuitButtonClicked);		
	}
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

	BindDelegates();
	AddInputMappingContext();
	SetInputModeGame();
}

void ATakeshiPlayerController::BindDelegates()
{
	TakeshiCharacter->OnReactToHazard.AddDynamic(this, &ATakeshiPlayerController::ReactToHazard);
	TakeshiPlayerState->OnPlayerLivesChanged.AddDynamic(this, &ATakeshiPlayerController::PlayerLivesChanged);
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

void ATakeshiPlayerController::MainMenuPlayButtonClicked()
{
	OnMainMenuPlayButtonClicked.Broadcast();
}

void ATakeshiPlayerController::MainMenuQuitButtonClicked()
{
	OnMainMenuQuitButtonClicked.Broadcast();
}
