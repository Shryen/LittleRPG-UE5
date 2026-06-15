#pragma once

#include "CoreMinimal.h"
#include "Actor/Interactable/InteractableObject.h"
#include "ResourceNode.generated.h"

class USphereComponent;
class UResourceData;

UCLASS()
class LITTLERPG_API AResourceNode : public AInteractableObject
{
	GENERATED_BODY()

public:
	AResourceNode();
	
	virtual void Interact(AActor* Interactor) override;
	
	
	UResourceData* GetResourceType() const { return ResourceType; }

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
	float RespawnTime = 0.f;
	
	float Health = 50.f;
};
