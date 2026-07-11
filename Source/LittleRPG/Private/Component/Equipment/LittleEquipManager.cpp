#include "Component/Equipment/LittleEquipManager.h"

#include "Actor/Equipment/Equipment.h"
#include "Actor/Equipment/Weapon/LittleWeaponBase.h"
#include "Character/LittleBaseCharacter.h"
#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Data/Inventory/ItemDataRow.h"
#include "PlayerState/LittlePlayerState.h"


ULittleEquipManager::ULittleEquipManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


bool ULittleEquipManager::SetupPlayerState()
{
	ALittleBaseCharacter* BaseCharacter = Cast<ALittleBaseCharacter>(GetOwner());
	if (!BaseCharacter) { UE_LOG(LogTemp, Error, TEXT("ULittleEquipManager::BeginPlay: Owner is not BaseCharacter.")); return false;}
	
	BaseCharacter->OnPlayerStateReady.AddUObject(this, &ULittleEquipManager::OnPlayerStateReady);
	
	if (BaseCharacter->GetPlayerState())
	{
		OnPlayerStateReady();
	}
	return true;
}

AActor* ULittleEquipManager::SpawnEquippedActor(EEquipmentSlot Slot, FName ItemRowName)
{
	if (!GetOwner()->HasAuthority()) return nullptr;
	
	UDataTable* DataTable = InventoryManager->GetItemDataTable();
	FItemDataRow* Row = DataTable->FindRow<FItemDataRow>(ItemRowName, "");
	if (!Row || !Row->ItemClass) return nullptr;
	
	AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(Row->ItemClass, FTransform::Identity, GetOwner());
	if (!NewActor) return nullptr;
	AEquipment* Equipment = Cast<AEquipment>(NewActor);
	if (!Equipment) return nullptr;
	
	FName Socket = Row->AttachmentSocket.IsNone() ? GetDefaultSocket(Slot) : Row->AttachmentSocket;
	ALittleBaseCharacter* BaseCharacter = Cast<ALittleBaseCharacter>(GetOwner());
	Equipment->AttachToComponent(BaseCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
	Equipment->TurnOffCollision();
	Equipment->FinishSpawning(FTransform::Identity);
	return NewActor;
}

FName ULittleEquipManager::GetDefaultSocket(EEquipmentSlot Slot) const
{
	switch (Slot)
	{
	case EEquipmentSlot::Weapon:   return TEXT("hand_rSocket");
	case EEquipmentSlot::OffHand:  return TEXT("hand_lSocket");
	case EEquipmentSlot::Head:     return TEXT("headSocket");
	case EEquipmentSlot::Chest:    return TEXT("spine_03Socket");
	case EEquipmentSlot::Legs:     return TEXT("pelvisSocket");
	case EEquipmentSlot::Feet:     return TEXT("foot_rSocket");
	case EEquipmentSlot::Ring:     return TEXT("hand_rSocket");
	case EEquipmentSlot::Trinket:  return TEXT("neckSocket");
	case EEquipmentSlot::Lantern:  return TEXT("lanternSocket");
	default:                       return NAME_None;
	}
}

void ULittleEquipManager::DestroyEquippedActor(EEquipmentSlot Slot)
{
	if (TObjectPtr<AActor>* Found = SpawnedEquipment.Find(Slot))
	{
		if (*Found)
			(*Found)->Destroy();
		SpawnedEquipment.Remove(Slot);
	}
}

void ULittleEquipManager::BeginPlay()
{
	Super::BeginPlay();
	if (!SetupPlayerState()) return;
}

void ULittleEquipManager::OnEquipmentChanged(const FEquipmentDisplayPayload& Payload)
{
	DestroyEquippedActor(Payload.SlotType);
	
	ALittleBaseCharacter* BaseCharacter = Cast<ALittleBaseCharacter>(GetOwner());
	if (!BaseCharacter || !BaseCharacter->GetMesh()) return;
	
	if (Payload.ItemRowName.IsNone())
	{
		if (Payload.SlotType == EEquipmentSlot::Weapon && UnarmedAnimClass)
			BaseCharacter->GetMesh()->SetAnimInstanceClass(UnarmedAnimClass);
		return; 
	}
	
	if (!Payload.ItemRowName.IsNone())
		if (AActor* NewActor = SpawnEquippedActor(Payload.SlotType, Payload.ItemRowName))
		{
			SpawnedEquipment.Add(Payload.SlotType, NewActor);
			
			if (Payload.SlotType == EEquipmentSlot::Weapon)
				if (ALittleWeaponBase* Weapon = Cast<ALittleWeaponBase>(NewActor))
					if (Weapon->WeaponAnimClass)
						BaseCharacter->GetMesh()->SetAnimInstanceClass(Weapon->WeaponAnimClass);
		}
}

void ULittleEquipManager::OnPlayerStateReady()
{
	ALittleBaseCharacter* BaseCharacter = Cast<ALittleBaseCharacter>(GetOwner());
	LittlePlayerState = Cast<ALittlePlayerState>(BaseCharacter->GetPlayerState());
	
	InventoryManager = LittlePlayerState->GetInventoryManager();
	InventoryManager->OnEquipmentSlotDirty.AddUObject(this, &ULittleEquipManager::OnEquipmentChanged);
}

