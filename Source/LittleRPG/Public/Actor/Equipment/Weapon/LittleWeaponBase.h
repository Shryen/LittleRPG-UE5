#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Actor/Equipment/Equipment.h"
#include "LittleWeaponBase.generated.h"

class ULittleGameplayAbility;

UCLASS()
class LITTLERPG_API ALittleWeaponBase : public AEquipment
{
	GENERATED_BODY()

public:
	ALittleWeaponBase();

protected:
	UFUNCTION(BlueprintCallable)
	void HitScan(TArray<FHitResult>& OutHit, bool& bWasHit);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HitScanStart(FGameplayEffectSpecHandle EffectSpecHandle);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HitScanEnd();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> ActorsToIgnore;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<AActor>> HitActors;
	
	UPROPERTY(EditAnywhere)
	float HitScanRadius = 20.f;
private:

	UPROPERTY(EditDefaultsOnly, Category = "Little|Weapon")
	TObjectPtr<USceneComponent> TraceStart;
	UPROPERTY(EditDefaultsOnly, Category = "Little|Weapon")
	TObjectPtr<USceneComponent> TraceEnd;
};
