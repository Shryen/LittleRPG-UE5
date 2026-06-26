#include "Actor/PickupObject.h"

#include "Character/LittleBaseCharacter.h"
#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerState/LittlePlayerState.h"


APickupObject::APickupObject()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	SetNetUpdateFrequency(10);
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	
	SphereCollision->InitSphereRadius(50.f);
	
	SphereCollision->SetCollisionEnabled(
		ECollisionEnabled::QueryOnly);

	SphereCollision->SetCollisionObjectType(
		ECC_GameTraceChannel1); // Pickup ObjectType
	
	SphereCollision->SetCollisionResponseToAllChannels(
	ECR_Ignore); // We only want to respond for characters
	
	SphereCollision->SetCollisionResponseToChannel(
		ECC_Pawn,
		ECR_Overlap);

	SphereCollision->SetGenerateOverlapEvents(true);
}

void APickupObject::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupObject::OnOverlap);
	InitialLocation = GetActorLocation();
}


void APickupObject::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("No authority!"));
		return;
	}
	
	ALittleBaseCharacter* Character = Cast<ALittleBaseCharacter>(OtherActor);
	if (Character)
	{
		ALittlePlayerState* PlayerState = Cast<ALittlePlayerState>(Character->GetPlayerState());
		Interact(Character);
		if (PlayerState)
		{
			PlayerState->GetInventoryManager()->AddItemToInventory(ItemRowName, Quantity);
			Destroy();
		}
	}
}

