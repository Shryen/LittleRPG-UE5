#include "PlayerState/LittlePlayerState.h"

#include "Component/InventoryManager/LittleInventoryManagerComponent.h"

ALittlePlayerState::ALittlePlayerState()
{
	bReplicates = true;
	SetNetUpdateFrequency(100.0f); 
	SetMinNetUpdateFrequency(33.0f);

	InventoryManager = CreateDefaultSubobject<ULittleInventoryManagerComponent>("InventoryManager");
}
