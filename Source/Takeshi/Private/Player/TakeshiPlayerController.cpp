// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "Player/TakeshiPlayerController.h"

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

	TakeshiPlayerState = GetPlayerState<ATakeshiPlayerState>();

	check(TakeshiPlayerState);
	check(TakeshiContext);

	TakeshiPlayerState->OnPlayerLivesChanged.AddDynamic(this, &ATakeshiPlayerController::PlayerLivesChanged);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(TakeshiContext, 0);
	}

	bShowMouseCursor = false;

	const FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);

	OnInitialized.Broadcast();
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

	if (InPawn != nullptr)
	{
		ATakeshiCharacterBase* TakeshiCharacterBase = CastChecked<ATakeshiCharacterBase>(InPawn);

		TakeshiCharacterBase->OnReactToHazard.AddDynamic(this, &ATakeshiPlayerController::ReactToHazard);
	}
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

void ATakeshiPlayerController::InitialisePlayerLives(const int32 InPlayerLives)
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
