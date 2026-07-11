#include "AnimInstance/LittleAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Character/LittleBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void ULittleAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Character = Cast<ALittleBaseCharacter>(TryGetPawnOwner());
	MovementComponent = Character ? Character->GetCharacterMovement() : nullptr;
}

void ULittleAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!Character || !MovementComponent)
		return;
	
	CharacterState = Character->GetCharacterState();
	
	Velocity = MovementComponent->Velocity;
	Speed = Velocity.Size2D();
	
	bShouldMove = Speed > 0.01f &&
		!MovementComponent->GetCurrentAcceleration().IsNearlyZero();
	
	bIsFalling = MovementComponent->IsFalling();
	
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Character->GetActorRotation());

	if (MovementComponent->bOrientRotationToMovement)
		Direction = FMath::Clamp(Direction, -45, 45);
}


