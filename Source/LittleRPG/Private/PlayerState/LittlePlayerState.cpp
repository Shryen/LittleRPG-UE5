#include "PlayerState/LittlePlayerState.h"

#include "Component/InventoryManager/LittleInventoryManagerComponent.h"

ALittlePlayerState::ALittlePlayerState()
{
	bReplicates = true;

	InventoryManager = CreateDefaultSubobject<ULittleInventoryManagerComponent>("InventoryManager");
}
