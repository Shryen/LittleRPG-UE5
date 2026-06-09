#include "Controller/PlayerController/LittlePlayerController.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputCoreTypes.h"
#include "Character/LittlePlayerCharacter.h"
#include "Component/StatComponent/LittleStatComponent.h"

ALittlePlayerController::ALittlePlayerController()
{
}

void ALittlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(MovementMappingContext, 0);
		}
	}
}

void ALittlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInput =
	  Cast<UEnhancedInputComponent>(InputComponent);

	if (!EnhancedInput)
	{
		return;
	}

	EnhancedInput->BindAction(
		MoveAction,
		ETriggerEvent::Triggered,
		this,
		&ALittlePlayerController::Move);

	EnhancedInput->BindAction(
		LookAction,
		ETriggerEvent::Triggered,
		this,
		&ALittlePlayerController::Look);
	
	EnhancedInput->BindAction(
	TakeDamageTest,
	ETriggerEvent::Triggered,
	this,
	&ALittlePlayerController::TestDamage);
}

void ALittlePlayerController::Move(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

		const FVector2D MovementVector = Value.Get<FVector2D>();

		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDirection =
			FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection =
			FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
}

void ALittlePlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(-LookAxisVector.Y);
}

void ALittlePlayerController::TestDamage()
{
		ALittlePlayerCharacter* CharacterPawn = Cast<ALittlePlayerCharacter>(GetPawn());
		if (CharacterPawn)
		{
			CharacterPawn->GetStatComponent()->ServerTakeDamage(10);
		}
}


