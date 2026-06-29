#pragma once

#include "CoreMinimal.h"
#include "Actor/Interactable/InteractableObject.h"
#include "ResourceNode.generated.h"

class USphereComponent;
class UResourceData;
class UInstancedStaticMeshComponent;

USTRUCT()
struct FDestroyedISMCInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FName ComponentName;

	UPROPERTY()
	int32 InstanceIndex = -1;
};

UCLASS()
class LITTLERPG_API AResourceNode : public AInteractableObject
{
	GENERATED_BODY()

public:
	AResourceNode();
	
	virtual void Interact(AActor* Interactor) override;
	
	UResourceData* GetResourceType() const { return ResourceType; }
	
	void DestroyNode();
	void RespawnNode();
	
	void SetDestroyedISMCInfo(UInstancedStaticMeshComponent* Component, int32 InstanceIndex);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UResourceData> ResourceType;
	
	UPROPERTY(EditDefaultsOnly)
	int32 ResourceAmount = 5.f;
	
	UPROPERTY(EditDefaultsOnly)
	float RespawnTime = 5.f;
	
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	float Health;
	
	FTimerHandle RespawnTimerHandle;
	
	UPROPERTY(ReplicatedUsing = OnRep_ToggleResourceNode)
	bool bIsActive = true;
	
	UPROPERTY(ReplicatedUsing = OnRep_DestroyedISMC)
	FDestroyedISMCInfo DestroyedISMCInfo;
	
	UFUNCTION()
	void OnRep_ToggleResourceNode();
	
	UFUNCTION()
	void OnRep_Health();
	
	UFUNCTION()
	void OnRep_DestroyedISMC();
};
