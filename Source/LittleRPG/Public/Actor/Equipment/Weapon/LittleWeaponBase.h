#pragma once

#include "CoreMinimal.h"
#include "Actor/Equipment/Equipment.h"
#include "LittleWeaponBase.generated.h"

UCLASS()
class LITTLERPG_API ALittleWeaponBase : public AEquipment
{
	GENERATED_BODY()

public:
	ALittleWeaponBase();
	
	UPROPERTY(EditDefaultsOnly, Category = "Little|Weapon")
	TSubclassOf<UAnimInstance> WeaponAnimClass;

protected:
	UFUNCTION(BlueprintCallable)
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Little|Weapon")
	TArray<AActor*> ActorsToIgnore;
};
