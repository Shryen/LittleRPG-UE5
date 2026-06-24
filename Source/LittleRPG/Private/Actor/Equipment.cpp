#include "Actor/Equipment.h"

#include "Character/LittlePlayerCharacter.h"
#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Components/BoxComponent.h"
#include "Data/ItemData.h"
#include "PlayerState/LittlePlayerState.h"


AEquipment::AEquipment()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
	StaticMesh->SetCollisionProfileName("NoCollision");
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(StaticMesh);
	BoxCollision->SetCollisionProfileName("OverlapAllDynamic");	
	BoxCollision->SetGenerateOverlapEvents(true);
}

void AEquipment::TurnOffCollision()
{
	BoxCollision->SetGenerateOverlapEvents(false);
	BoxCollision->SetCollisionProfileName("NoCollision");
	SetActorEnableCollision(false);
}

void AEquipment::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AEquipment::BeginPlay: Item [%s] was added to scene!"), *GetName());
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AEquipment::OnOverlapBegin);
}

void AEquipment::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	if (!OtherActor || OtherActor == this) return;
	if (!ItemData) return;
		
	ALittlePlayerCharacter* PlayerCharacter = Cast<ALittlePlayerCharacter>(OtherActor);
	if (!PlayerCharacter) return;
	ALittlePlayerState* PS = Cast<ALittlePlayerState>(PlayerCharacter->GetPlayerState());
	if (!PS) return;
	PS->GetInventoryManager()->AddItemToInventory(ItemData);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerCharacter;
	SpawnParams.Instigator = PlayerCharacter;
	
	AActor* NewEquipment = GetWorld()->SpawnActor<AActor>(ItemData->ItemClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (!NewEquipment) return;
	NewEquipment->SetOwner(PS);
	NewEquipment->SetActorEnableCollision(false);

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
	NewEquipment->AttachToComponent(PlayerCharacter->GetLightMagicSceneComponent(), AttachRules);

	PS->GetInventoryManager()->Server_EquipItem(ItemData, NewEquipment);

	Destroy();
}


