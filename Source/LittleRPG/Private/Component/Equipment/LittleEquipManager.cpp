#include "Component/Equipment/LittleEquipManager.h"
#include "Actor/Equipment/Equipment.h"
#include "Actor/Equipment/Weapon/LittleWeaponBase.h"
#include "Character/LittleBaseCharacter.h"
#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Data/Inventory/ItemDataRow.h"
#include "PlayerState/LittlePlayerState.h"

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
	
	AActor* NewActor = SpawnEquippedActor(Payload.SlotType, Payload.ItemRowName);
	if (!NewActor) return;
	
	SpawnedEquipment.Add(Payload.SlotType, NewActor);
	ALittleWeaponBase* Weapon = Cast<ALittleWeaponBase>(NewActor);
	if (Payload.SlotType == EEquipmentSlot::Weapon && Weapon && Weapon->WeaponAnimClass)
		BaseCharacter->GetMesh()->SetAnimInstanceClass(Weapon->WeaponAnimClass);
}

void ULittleEquipManager::BeginPlay()
{
	Super::BeginPlay();
	ALittleBaseCharacter* BaseCharacter = Cast<ALittleBaseCharacter>(GetOwner());
	AbilitySystemComponent = BaseCharacter ? BaseCharacter->GetAbilitySystemComponent() : nullptr;
	check(AbilitySystemComponent);
	if (!SetupPlayerState()) return;
}

void ULittleEquipManager::OnPlayerStateReady()
{
	if (InventoryManager) return;
	ALittleBaseCharacter* BaseCharacter = Cast<ALittleBaseCharacter>(GetOwner());
	LittlePlayerState = Cast<ALittlePlayerState>(BaseCharacter->GetPlayerState());
	
	InventoryManager = LittlePlayerState->GetInventoryManager();
	InventoryManager->OnEquipmentSlotDirty.AddUObject(this, &ULittleEquipManager::OnEquipmentChanged);
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

FItemDataRow* ULittleEquipManager::GetRowFromDataTable(FName ItemRowName) const
{
	UDataTable* DataTable = InventoryManager->GetItemDataTable();
	return DataTable->FindRow<FItemDataRow>(ItemRowName, TEXT(""));
}

AEquipment* ULittleEquipManager::DefferEquipment(const FItemDataRow* Row) const
{
	AEquipment* Equipment = GetWorld()->SpawnActorDeferred<AEquipment>(
		Row->ItemClass,
		FTransform::Identity,
		GetOwner()
	);
	return Equipment;
}

void ULittleEquipManager::AttachToMesh(const EEquipmentSlot Slot, const FItemDataRow* Row, AEquipment* Equipment) const
{
	if (!Row || !Equipment) return;
	FName Socket = Row->AttachmentSocket.IsNone() ? GetDefaultSocket(Slot) : Row->AttachmentSocket;
	ALittleBaseCharacter* BaseCharacter = Cast<ALittleBaseCharacter>(GetOwner());
	checkf(BaseCharacter, TEXT("BaseCharacter not found when trying to attach to it's mesh"));
	Equipment->AttachToComponent(BaseCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
}

void ULittleEquipManager::DestroyEquippedActor(const EEquipmentSlot Slot)
{
	if (GetOwner()->HasAuthority() && AbilitySystemComponent)
	{
		if (TArray<FActiveGameplayEffectHandle>* Handles = ActiveGameplayEffects.Find(Slot))
			for (FActiveGameplayEffectHandle& Handle : *Handles)
				AbilitySystemComponent->RemoveActiveGameplayEffect(Handle);
	}
	ActiveGameplayEffects.Remove(Slot);

	if (TObjectPtr<AActor>* Found = SpawnedEquipment.Find(Slot))
	{
		if (*Found)
			(*Found)->Destroy();
		SpawnedEquipment.Remove(Slot);
	}
}


AActor* ULittleEquipManager::SpawnEquippedActor(const EEquipmentSlot Slot, const FName ItemRowName)
{
	if (!GetOwner()->HasAuthority()) return nullptr;
	
	FItemDataRow* Row = GetRowFromDataTable(ItemRowName);
	if (!Row || !Row->ItemClass)
		return nullptr;
	
	
	AEquipment* Equipment = DefferEquipment(Row);
	if (!Equipment)
		return nullptr;
		
	Equipment->TurnOffCollision();
	Equipment->FinishSpawning(FTransform::Identity);
	AttachToMesh(Slot, Row, Equipment);
	
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(Equipment);
	for (const TSubclassOf<UGameplayEffect>& GE : Row->GivenStats)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GE, 1, ContextHandle);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			ActiveGameplayEffects.FindOrAdd(Slot).Add(ActiveHandle);
		}
	}
	return Equipment;
}

FName ULittleEquipManager::GetDefaultSocket(const EEquipmentSlot Slot) const
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

