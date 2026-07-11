#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "EffectAreaBase.generated.h"

class UBoxComponent;
class UGameplayEffect;

UCLASS()
class LITTLERPG_API AEffectAreaBase : public AActor
{
	GENERATED_BODY()

public:
	AEffectAreaBase();

protected:
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Little|EffectArea")
	TSubclassOf<UGameplayEffect> Effect;
	
	UPROPERTY()
	TObjectPtr<UBoxComponent> Collision;
	
	FGameplayEffectSpecHandle SpecHandle;
	
	UPROPERTY(EditDefaultsOnly, Category="Little|EffectArea")
	float Magnitude;
	
	UPROPERTY(EditDefaultsOnly, Category="Little|EffectArea")
	FGameplayTag GameplayTag;
};
