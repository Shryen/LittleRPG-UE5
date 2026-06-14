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

		Super::Interact(Interactor);
		Health -= 10.f;

		APawn* Pawn = Cast<APawn>(Interactor);
		if (!Pawn) return;

		ALittlePlayerState* PS = Pawn->GetPlayerState<ALittlePlayerState>();
		if (!PS) return;

		if (!ResourceType || !ResourceType->DropsItem) return;

		UItemData* Item = NewObject<UItemData>(this, ResourceType->DropsItem);
		PS->AddItem(Item);

		if (Health <= 0.f)
		{
			// TODO: trigger respawn timer, play death effect, hide mesh
			UE_LOG(LogTemp, Warning, TEXT("AResourceNode: %s depleted"), *GetName());
		}
	}

void AResourceNode::BeginPlay()
{
	Super::BeginPlay();
	
}

