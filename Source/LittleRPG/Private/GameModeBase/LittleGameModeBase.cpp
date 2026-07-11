#include "GameModeBase/LittleGameModeBase.h"

#include "Character/LittlePlayerCharacter.h"
#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Controller/PlayerController/LittlePlayerController.h"
#include "Data/Inventory/FInventorySlot.h"
#include "Data/Save/LittleSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/LittlePlayerState.h"

ALittleGameModeBase::ALittleGameModeBase()
{
	PlayerControllerClass = ALittlePlayerController::StaticClass();
	DefaultPawnClass = ALittlePlayerCharacter::StaticClass();
}

void ALittleGameModeBase::SaveGame() const
{
	APlayerState* PS = GetWorld()->GetFirstPlayerController()->PlayerState;
	ALittlePlayerState* LPS = Cast<ALittlePlayerState>(PS);
	if (!LPS) return;
	
	ULittleSaveGame* SaveData = Cast<ULittleSaveGame>(UGameplayStatics::CreateSaveGameObject(ULittleSaveGame::StaticClass()));
	SaveData->SavedNextSlotID = LPS->GetInventoryManager()->GetNextSlotID();

	for (const FInventorySlot& InventorySlot : LPS->GetInventoryManager()->GetInventory().Items)
	{
		if (InventorySlot.ItemRowName.IsNone()) continue;

		FSavedInventorySlot Saved;
		Saved.SlotID = InventorySlot.SlotID;
		Saved.ItemRowName = InventorySlot.ItemRowName;
		Saved.Quantity     = InventorySlot.Quantity;
		
		SaveData->SavedInventory.Add(Saved);
	}
	
	UGameplayStatics::SaveGameToSlot(SaveData, SaveSlotName, UserIndex);
	UE_LOG(LogTemp, Warning, TEXT("Game saved — %d slots"), SaveData->SavedInventory.Num());
}

void ALittleGameModeBase::LoadGame(ALittlePlayerState* PlayerState) const
{
	if (!PlayerState) return;
	
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("No save file found"));
		return;
	}
	
	ULittleSaveGame* SaveData = Cast<ULittleSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
	if (!SaveData) return;
	
}

void ALittleGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle SaveTimerHandle;
	GetWorldTimerManager().SetTimer(SaveTimerHandle, this, 
		&ALittleGameModeBase::SaveGame, 60.f, true); 
}

void ALittleGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	if (ALittlePlayerState* PS = NewPlayer->GetPlayerState<ALittlePlayerState>())
	{
		LoadGame(PS);
	}
}


