
#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableObject.h"
#include "PickupObject.generated.h"

class USphereComponent;
class UItemData;

UCLASS()
class LITTLERPG_API APickupObject : public AInteractableObject
{
	GENERATED_BODY()

public:
	APickupObject();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LittleRPG | Data")
	TObjectPtr<UItemData> ItemData;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> SphereCollision;
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);
	
	
	FVector InitialLocation;
};
