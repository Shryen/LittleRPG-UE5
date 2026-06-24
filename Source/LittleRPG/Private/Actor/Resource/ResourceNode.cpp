#include "Actor/Resource/ResourceNode.h"

#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Data/ResourceData.h"
#include "PlayerState/LittlePlayerState.h"


AResourceNode::AResourceNode()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

void AResourceNode::Respawn()
{
	if (!HasAuthority()) return;
	Mesh->SetVisibility(true);
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));
	SetActorEnableCollision(true);
	GetWorldTimerManager().ClearTimer(RespawnTimerHandle);
	Health = 50.f;
}

void AResourceNode::HideHarvestedMesh()
{
	if (!HasAuthority()) return;
	Mesh->SetVisibility(false);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	SetActorEnableCollision(false);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AResourceNode::Respawn, RespawnTime, false);
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
	PS->GetInventoryManager()->AddItemToInventory(ResourceType->DropsItem);

	if (Health <= 0.f)
		HideHarvestedMesh();
}



void AResourceNode::BeginPlay()
{
	Super::BeginPlay();
}

