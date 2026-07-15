#include "Component/Equipment/LittleEquipManager.h"
#include "Actor/Equipment/Equipment.h"
#include "Actor/Equipment/Weapon/LittleWeaponBase.h"
#include "Character/LittleBaseCharacter.h"
#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Data/Inventory/ItemDataRow.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/LittlePlayerState.h"

ULittleEquipManager::ULittleEquipManager()
{
	SetIsReplicatedByDefault(true);
}

void ULittleEquipManager::OnEquipmentChanged(const FEquipmentDisplayPayload& EquipmentDisplayPayload)
{
	if (!GetOwner()->HasAuthority()) return;
	DestroyEquippedActor(EquipmentDisplayPayload.SlotType);
	
	if (!BaseCharacter || !BaseCharacter->GetMesh()) return;
	
	if (EquipmentDisplayPayload.ItemRowName.IsNone())
	{
		if (EquipmentDisplayPayload.SlotType == EEquipmentSlot::Weapon && UnarmedAnimClass)
			UpdateAnimInstanceClass(UnarmedAnimClass);
			//BaseCharacter->GetMesh()->SetAnimInstanceClass(UnarmedAnimClass);
		return; 
	}
	
	AActor* NewActor = SpawnEquippedActor(EquipmentDisplayPayload.SlotType, EquipmentDisplayPayload.ItemRowName);
	if (!NewActor) return;
	
	SpawnedEquipment.Add(EquipmentDisplayPayload.SlotType, NewActor);
	ALittleWeaponBase* Weapon = Cast<ALittleWeaponBase>(NewActor);
	FItemDataRow* Row = GetRowFromDataTable(EquipmentDisplayPayload.ItemRowName);
	if (!Row) return;
	if (EquipmentDisplayPayload.SlotType == EEquipmentSlot::Weapon && Weapon && Row->EquipmentConfig.AnimInstance)
		UpdateAnimInstanceClass(Row->EquipmentConfig.AnimInstance);
		//BaseCharacter->GetMesh()->SetAnimInstanceClass(Weapon->WeaponAnimClass);
	
}

void ULittleEquipManager::UpdateAnimInstanceClass(TSubclassOf<UAnimInstance> NewClass)
{
	CurrentAnimInstanceClass = NewClass;
	OnRep_AnimInstance(CurrentAnimInstanceClass);
}

void ULittleEquipManager::OnRep_AnimInstance(TSubclassOf<UAnimInstance> OldAnimInstance) const
{
	if (CurrentAnimInstanceClass == nullptr) return;
	if (!BaseCharacter || !BaseCharacter->GetMesh()) return;
	USkeletalMeshComponent* Mesh = BaseCharacter->GetMesh();
	Mesh->SetAnimInstanceClass(CurrentAnimInstanceClass);
}

void ULittleEquipManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULittleEquipManager, CurrentAnimInstanceClass);
}

void ULittleEquipManager::BeginPlay()
{
	Super::BeginPlay();
	BaseCharacter = Cast<ALittleBaseCharacter>(GetOwner());
	AbilitySystemComponent = BaseCharacter ? BaseCharacter->GetAbilitySystemComponent() : nullptr;
	check(AbilitySystemComponent);
	if (!SetupPlayerState()) return;
}

ALittleWeaponBase* ULittleEquipManager::GetEquippedWeapon()
{
	TObjectPtr<AActor>* Equipment = SpawnedEquipment.Find(EEquipmentSlot::Weapon);
	if (!Equipment || !*Equipment) return nullptr;
	
	ALittleWeaponBase* Weapon = Cast<ALittleWeaponBase>(*Equipment);
	return Weapon;
}

void ULittleEquipManager::OnPlayerStateReady()
{
	if (InventoryManager) return;
	LittlePlayerState = Cast<ALittlePlayerState>(BaseCharacter->GetPlayerState());
	
	InventoryManager = LittlePlayerState->GetInventoryManager();
	InventoryManager->OnEquipmentSlotDirty.AddUObject(this, &ULittleEquipManager::OnEquipmentChanged);
}

bool ULittleEquipManager::SetupPlayerState()
{
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
	APawn* Instigator = Cast<APawn>(GetOwner());
	AEquipment* Equipment = GetWorld()->SpawnActorDeferred<AEquipment>(
		Row->ItemClass,
		FTransform::Identity,
		GetOwner(),
		Instigator
	);
	return Equipment;
}

void ULittleEquipManager::AttachToMesh(const EEquipmentSlot Slot, const FItemDataRow* Row, AEquipment* Equipment) const
{
	if (!Row || !Equipment) return;
	FName Socket = Row->AttachmentSocket.IsNone() ? GetDefaultSocket(Slot) : Row->AttachmentSocket;
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
	
	RemoveAbilitiesGrantedFromEquipment();
}

void ULittleEquipManager::ApplyGameplayEffectFromEquipment(const EEquipmentSlot Slot, FItemDataRow* Row, const AEquipment* Equipment)
{
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(Equipment);
	for (const TSubclassOf<UGameplayEffect>& GE : Row->EquipmentConfig.EffectsToGrant)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GE, 1, ContextHandle);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			ActiveGameplayEffects.FindOrAdd(Slot).Add(ActiveHandle);
		}
	}
}

TArray<FGameplayAbilitySpecHandle> ULittleEquipManager::GrantAbilitiesFromEquipment(const FItemDataRow* Row)
{
	if (!BaseCharacter) return TArray<FGameplayAbilitySpecHandle>();
	GrantedAbilities = BaseCharacter->GrantAbilities(Row->EquipmentConfig.AbilitiesToGrant);
	return GrantedAbilities;
}

void ULittleEquipManager::RemoveAbilitiesGrantedFromEquipment()
{
	if (!BaseCharacter) return;
	BaseCharacter->RemoveAbilities(GrantedAbilities);
	GrantedAbilities.Empty();
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
	
	ApplyGameplayEffectFromEquipment(Slot, Row, Equipment);
	GrantAbilitiesFromEquipment(Row);
	
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