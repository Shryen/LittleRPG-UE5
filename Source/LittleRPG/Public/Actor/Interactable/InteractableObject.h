
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "InteractableObject.generated.h"


UCLASS()
class LITTLERPG_API AInteractableObject : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AInteractableObject();
	
	virtual void Interact(AActor* Interactor) override;


protected:
	virtual FText GetInteractText() override {return InteractText;}; 
	
private:
	FText InteractText;
};
