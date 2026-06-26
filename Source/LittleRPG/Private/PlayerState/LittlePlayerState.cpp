#include "PlayerState/LittlePlayerState.h"

#include "Component/InventoryManager/LittleInventoryManagerComponent.h"

ALittlePlayerState::ALittlePlayerState()
{
	bReplicates = true;
	NetUpdateFrequency = 100.0f; 
	MinNetUpdateFrequency = 33.0f;

	InventoryManager = CreateDefaultSubobject<ULittleInventoryManagerComponent>("InventoryManager");
}
