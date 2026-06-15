#include "Actor/Interactable/InteractableObject.h"

AInteractableObject::AInteractableObject()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
	SetReplicateMovement(false);
}

void AInteractableObject::Interact(AActor* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacted with object: %s by %s"), *GetName(), *Interactor->GetName());
}

