#include "Actor/Resource/ResourceNode.h"

#include "Data/ResourceData.h"
#include "PlayerState/LittlePlayerState.h"


AResourceNode::AResourceNode()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

void AResourceNode::Interact(AActor* Interactor)
{
	Super::Interact(Interactor);
	if (Health <= 0.f) return;

	Health -= 10.f;

	APawn* Pawn = Cast<APawn>(Interactor);
	if (!Pawn) return;

	ALittlePlayerState* PS = Pawn->GetPlayerState<ALittlePlayerState>();
	if (!PS) return;

	if (!ResourceType || !ResourceType->DropsItem) return;
	PS->AddItemToInventory(ResourceType->DropsItem);

	if (Health <= 0.f)
		UE_LOG(LogTemp, Warning, TEXT("AResourceNode: %s depleted"), *GetName());
}



void AResourceNode::BeginPlay()
{
	Super::BeginPlay();
	
}

