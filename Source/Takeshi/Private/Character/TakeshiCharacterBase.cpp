// Copyright Rob Meade & Ethan Mitchell.  All Rights Reserved.

#include "Character/TakeshiCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Interaction/CharacterInteractionInterface.h"


ATakeshiCharacterBase::ATakeshiCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->GroundFriction = 8.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;	
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ATakeshiCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	DefaultBrakingDecelerationWalking = GetCharacterMovement()->BrakingDecelerationWalking;
	DefaultGroundFriction = GetCharacterMovement()->GroundFriction;
}

void ATakeshiCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckFloorForCharacterInteraction();
}

void ATakeshiCharacterBase::CheckFloorForCharacterInteraction()
{
	FFindFloorResult FloorResult;
	GetCharacterMovement()->FindFloor(GetCapsuleComponent()->GetComponentLocation(), FloorResult, true);

	if (FloorResult.HitResult.GetActor() != LastFloorHitActor)
	{
		if (FloorResult.HitResult.bBlockingHit)
		{
			if (LastFloorHitActor)
			{
				if (ICharacterInteractionInterface* CharacterInteractionInterface = Cast<ICharacterInteractionInterface>(LastFloorHitActor))
				{
					CharacterInteractionInterface->CharacterInteractionEnd(this);
				}
			}

			if (ICharacterInteractionInterface* CharacterInteractionInterface = Cast<ICharacterInteractionInterface>(FloorResult.HitResult.GetActor()))
			{
				CharacterInteractionInterface->CharacterInteractionBegin(this);
			}

			LastFloorHitActor = FloorResult.HitResult.GetActor();
		}
		else
		{
			if (ICharacterInteractionInterface* CharacterInteractionInterface = Cast<ICharacterInteractionInterface>(LastFloorHitActor))
			{
				CharacterInteractionInterface->CharacterInteractionEnd(this);
			}

			LastFloorHitActor = nullptr;
		}
	}
}

void ATakeshiCharacterBase::ReactToHazard()
{
	Destroy();
}

void ATakeshiCharacterBase::Destroyed()
{
	Super::Destroyed();
}
