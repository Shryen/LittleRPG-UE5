#include "GameModeBase/LittleGameModeBase.h"

#include "Character/LittlePlayerCharacter.h"
#include "Controller/PlayerController/LittlePlayerController.h"
#include "Data/FInventorySlot.h"
#include "Data/ItemData.h"
#include "Data/LittleSaveGame.h"
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
	SaveData->SavedNextSlotID = LPS->GetNextSlotID();

	for (const FInventorySlot& InventorySlot : LPS->GetInventory())
	{
		if (!InventorySlot.ItemData) continue;
		
		FSavedInventorySlot Saved;
		Saved.SlotID = InventorySlot.SlotID;
		Saved.ItemDataPath = FSoftObjectPath(InventorySlot.ItemData);
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
	
	PlayerState->SetNextSlotID(SaveData->SavedNextSlotID);
	
	for (const FSavedInventorySlot& SavedSlot : SaveData->SavedInventory)
	{
		UItemData* Item = Cast<UItemData>(SavedSlot.ItemDataPath.TryLoad());
		if (!Item)
		{
			UE_LOG(LogTemp, Warning, TEXT("Item Data is null"));
			continue;
		}
		
		FInventorySlot NewSlot;
		NewSlot.SlotID    = SavedSlot.SlotID;
		NewSlot.ItemData  = Item;
		NewSlot.Quantity  = SavedSlot.Quantity;
		PlayerState->GetInventory().Add(NewSlot);
		PlayerState->Server_OnSlotChanged(NewSlot);
	}
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


