#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LittleProjectile.generated.h"

class UNiagaraComponent;
class UGameplayEffect;
class UArrowComponent;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class LITTLERPG_API ALittleProjectile : public AActor
{
	GENERATED_BODY()

public:
	ALittleProjectile();
	void InitProjectileMovementComponent();

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void BeginPlay() override;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Little|Projectile", meta=(ExposeOnSpawn))
	float ProjectileSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Little|Projectile", meta=(ExposeOnSpawn))
	FVector TargetLocation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Little|Projectile")
	TSubclassOf<UGameplayEffect> ProjectileEffect;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Little")
	TObjectPtr<USphereComponent> HitCollision;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little")
	TObjectPtr<UNiagaraComponent> Projectile;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little")
	TObjectPtr<UArrowComponent> ProjectileDirection;

};
