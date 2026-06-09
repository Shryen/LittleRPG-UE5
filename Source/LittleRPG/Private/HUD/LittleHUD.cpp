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
	
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	InventoryWidget = CreateWidget<UInventoryWidget>(PC, InventoryWidgetClass);
}


void ALittleHUD::ShowInventoryWidget() 
{
	if (!InventoryWidget)
		return;
	
	ALittleBaseCharacter* Character = Cast<ALittleBaseCharacter>(GetOwningPawn());
	if (!Character) return;
	
	ALittlePlayerState* PlayerState = Cast<ALittlePlayerState>(Character->GetPlayerState());
	if (!PlayerState) return;
	
	for (const UItemData* Item : PlayerState->GetInventory())
	{
		InventoryWidget->AddItem(Item->ItemName);
	}
	
	InventoryWidget->AddToViewport();
	bInventoryOpen = true;
}

void ALittleHUD::HideInventoryWidget() 
{
	if (!InventoryWidget)
		return;
	
	InventoryWidget->RemoveFromParent();
	InventoryWidget->ClearItems();
	bInventoryOpen = false;
}

void ALittleHUD::ToggleInventory()
{
	if (!bInventoryOpen)
		ShowInventoryWidget();
	else
		HideInventoryWidget();
}

