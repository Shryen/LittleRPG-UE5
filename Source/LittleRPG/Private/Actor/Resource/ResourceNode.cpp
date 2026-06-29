#include "Actor/Resource/ResourceNode.h"

#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Data/ResourceData.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
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
	if (!HasAuthority()) return;

	if (Health <= 0.f) return;

	Health = FMath::Clamp(Health -10, 0.f, 50.f);

	APawn* Pawn = Cast<APawn>(Interactor);
	if (!Pawn) return;

	ALittlePlayerState* PS = Pawn->GetPlayerState<ALittlePlayerState>();
	if (!PS) return;

	if (!ResourceType || ResourceType->DropsItemRowName.IsNone()) return;
	PS->GetInventoryManager()->AddItemToInventory(ResourceType->DropsItemRowName, ResourceAmount);
	
	if (Health <= 0.f)
		DestroyNode();
}

void AResourceNode::DestroyNode()
{
	if (!HasAuthority()) return;
	bIsActive = false;
	OnRep_ToggleResourceNode(); 
	
	GetWorldTimerManager().SetTimer(
	   RespawnTimerHandle,
	   this,
	   &AResourceNode::RespawnNode,
	   RespawnTime,
	   false);
}

void AResourceNode::RespawnNode()
{
	if (!HasAuthority()) return;

	bIsActive = true;
	OnRep_ToggleResourceNode(); 

	Health = 50.f;
}

void AResourceNode::BeginPlay()
{
	Super::BeginPlay();
	bIsActive = true;
	Health = 50.f;
}

void AResourceNode::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION_NOTIFY(AResourceNode, bIsActive, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(AResourceNode, Health);
	DOREPLIFETIME_CONDITION_NOTIFY(AResourceNode, DestroyedISMCInfo, COND_None, REPNOTIFY_Always);
}

void AResourceNode::OnRep_ToggleResourceNode()
{
	Mesh->SetVisibility(bIsActive);
	Mesh->SetCollisionProfileName(bIsActive ? TEXT("BlockAll") : TEXT("NoCollision"));
	SetActorEnableCollision(bIsActive);
}

void AResourceNode::OnRep_Health()
{
	UE_LOG(LogTemp, Warning, TEXT("Health of tree: %f"), Health);
}

void AResourceNode::SetDestroyedISMCInfo(UInstancedStaticMeshComponent* Component, int32 InstanceIndex)
{
	if (!HasAuthority()) return;
	if (!Component) return;
	
	UE_LOG(LogTemp, Warning, TEXT("[SetDestroyedISMCInfo] Component=%s InstanceIndex=%d IsValid=%d"),
		*Component->GetFName().ToString(), InstanceIndex, Component->IsValidInstance(InstanceIndex));

	DestroyedISMCInfo.ComponentName = Component->GetFName();
	DestroyedISMCInfo.InstanceIndex = InstanceIndex;

	Component->RemoveInstance(InstanceIndex);
	UE_LOG(LogTemp, Warning, TEXT("[SetDestroyedISMCInfo] Removed instance on server. Remaining instances=%d"),
		Component->GetNumRenderInstances());
}

void AResourceNode::OnRep_DestroyedISMC()
{
	UE_LOG(LogTemp, Warning, TEXT("[OnRep_DestroyedISMC] Called. ComponentName=%s InstanceIndex=%d HasAuthority=%d"),
		*DestroyedISMCInfo.ComponentName.ToString(), DestroyedISMCInfo.InstanceIndex, HasAuthority());

	if (HasAuthority()) return;
	if (DestroyedISMCInfo.InstanceIndex < 0) return;
	
	UWorld* World = GetWorld();
	if (!World) return;
	
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (UInstancedStaticMeshComponent* ISMC = 
			It->FindComponentByClass<UInstancedStaticMeshComponent>())
		{
			if (ISMC->GetFName() == DestroyedISMCInfo.ComponentName && 
				ISMC->IsValidInstance(DestroyedISMCInfo.InstanceIndex))
			{
				UE_LOG(LogTemp, Warning, TEXT("[OnRep_DestroyedISMC] Found ISMC on client, removing instance"));
				ISMC->RemoveInstance(DestroyedISMCInfo.InstanceIndex);
				return;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[OnRep_DestroyedISMC] ISMC not found on client!"));
}

