

#include "PlayerState/LittlePlayerState.h"

#include "Data/ItemData.h"
#include "Net/UnrealNetwork.h"

ALittlePlayerState::ALittlePlayerState()
{
	bReplicates = true;
}

void ALittlePlayerState::AddItem(UItemData* Item)
{
	if (!Item)
		return;
	
	if (!HasAuthority())
		return;
	
	Inventory.Add(Item);
	OnInventoryChanged.Broadcast();
	PrintInventory();
}

void ALittlePlayerState::PrintInventory()
{
	if (Inventory.IsEmpty())
	{
		return;
	}
	
	if (!HasAuthority())
		return;
	
	for (const UItemData* Item : Inventory)
	{
		UE_LOG(LogTemp, Warning, 
			TEXT("Item Name in Inventory: %s for Player: %s"), 
			*Item->ItemName.ToString(),
			*GetPlayerController()->GetName());
	}
}

void ALittlePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ALittlePlayerState, Inventory);
}

void ALittlePlayerState::OnRep_Inventory()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Inventory fired — items: %d"), Inventory.Num());
	OnInventoryChanged.Broadcast();
}
