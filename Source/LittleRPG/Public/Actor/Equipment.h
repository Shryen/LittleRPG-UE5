#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Equipment.generated.h"

class UBoxComponent;
class UItemData;

UCLASS()
class LITTLERPG_API AEquipment : public AActor
{
	GENERATED_BODY()

public:
	AEquipment();

	const FGameplayTag& GetEquippedStateTag() const { return EquippedStateTag; };
	
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
	TObjectPtr<UItemData> ItemData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TObjectPtr<UBoxComponent> BoxCollision;
	
	FGameplayTag EquippedStateTag;
};
