#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LittleAnimInstance.generated.h"

class ALittleBaseCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class LITTLERPG_API ULittleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LittleAnimInstance")
	TObjectPtr<ALittleBaseCharacter> Character;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LittleAnimInstance")
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LittleAnimInstance")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LittleAnimInstance")
	float Speed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LittleAnimInstance")
	float Direction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LittleAnimInstance")
	bool bShouldMove;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LittleAnimInstance")
	bool bIsFalling;
	
};
