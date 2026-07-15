#pragma once

#include "CoreMinimal.h"
#include "Actor/Pickup/PickupObject.h"
#include "Data/Equipment/EquipmentConfig.h"
#include "GameFramework/Actor.h"
#include "Equipment.generated.h"

struct FEquipmentConfig;
class UBoxComponent;

UCLASS()
class LITTLERPG_API AEquipment : public APickupObject
{
	GENERATED_BODY()

public:
	AEquipment();
	
	void TurnOffCollision();
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlapBegin(   UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TObjectPtr<UBoxComponent> BoxCollision;
};
