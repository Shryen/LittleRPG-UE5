#include "HUD/LittleHUD.h"

#include "Character/LittleBaseCharacter.h"
#include "Data/ItemData.h"
#include "HUD/Widget/Inventory/InventoryWidget.h"
#include "PlayerState/LittlePlayerState.h"

void ALittleHUD::BeginPlay()
{
	Super::BeginPlay();
	if (!InventoryWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALittleHUD: InventoryWidgetClass is not set! Set it in HUD!"));
		return;
	}
	
	APlayerController* PC = GetOwningPlayerController();
	InventoryWidget = CreateWidget<UInventoryWidget>(PC, InventoryWidgetClass);
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	InventoryWidget->AddToViewport();
	
	ALittleBaseCharacter* Character = Cast<ALittleBaseCharacter>(GetOwningPawn());
	checkf(Character, TEXT("ALittleHUD::BeginPlay: Character not found"));
	
	Character->OnPlayerStateReady.AddUObject(this, &ALittleHUD::OnPlayerStateReady);

	if (Character->GetPlayerState())
	{
		OnPlayerStateReady();
	}
}


void ALittleHUD::PopulateInventory() const
{
	ALittleBaseCharacter* Character = Cast<ALittleBaseCharacter>(GetOwningPawn());
	if (!Character) return;
	
	ALittlePlayerState* PlayerState = Cast<ALittlePlayerState>(Character->GetPlayerState());
	if (!PlayerState) return;
	
	InventoryWidget->ClearItems();
	
	for (const UItemData* Item : PlayerState->GetInventory())
	{
		if (!Item)
		{
			UE_LOG(LogTemp, Warning, TEXT("ALittleHUD::PopulateInventory — null item in inventory, skipping"));
			continue; 
		}
		InventoryWidget->AddItem(Item->ItemName);
	}
}

void ALittleHUD::OnPlayerStateReady()
{
	ALittleBaseCharacter* Character = Cast<ALittleBaseCharacter>(GetOwningPawn());
	checkf(Character, TEXT("ALittleHUD::OnPlayerState: Character not found"));
	ALittlePlayerState* PlayerState = Cast<ALittlePlayerState>(Character->GetPlayerState());
	checkf(PlayerState, TEXT("ALittleHUD::OnPlayerState:  PlayerState null after OnPlayerStateReady"));
	PlayerState->OnInventoryChanged.AddUObject(this, &ALittleHUD::PopulateInventory);
	PopulateInventory();
}


void ALittleHUD::ShowInventoryWidget() 
{
	if (!InventoryWidget)
		return;
	
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	bInventoryOpen = true;
}

void ALittleHUD::HideInventoryWidget() 
{
	if (!InventoryWidget)
		return;
	
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryOpen = false;
}

void ALittleHUD::ToggleInventory()
{
	if (!bInventoryOpen)
		ShowInventoryWidget();
	else
		HideInventoryWidget();
}

