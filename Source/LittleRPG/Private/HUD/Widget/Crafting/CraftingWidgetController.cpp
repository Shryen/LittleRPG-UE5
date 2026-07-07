#include "HUD/Widget/Crafting/CraftingWidgetController.h"
#include "Component/LittleCraftManagerComponent.h"
#include "HUD/Widget/Crafting/CraftingWidget.h"
#include "PlayerState/LittlePlayerState.h"

void UCraftingWidgetController::SetWidget(ULittleUserWidget* InWidget)
{
	Super::SetWidget(InWidget);
	CraftingWidget = Cast<UCraftingWidget>(InWidget);
	if (!CraftingWidget)
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidgetController::SetWidget: Cast to UInventoryWidget failed!"));
}

void UCraftingWidgetController::BindDependencies()
{
	if (!CraftingWidget) return;
	
	CraftingWidget->OnCraftRequested.AddUObject(this, &UCraftingWidgetController::CraftItem);
}

void UCraftingWidgetController::CraftItem(FName ItemRowName)
{
	ALittlePlayerState* LittlePlayerState = Cast<ALittlePlayerState>(PlayerState);
	if (!LittlePlayerState) return;
	
	ULittleCraftManagerComponent* CraftManager = LittlePlayerState->GetCraftManager();
	CraftManager->Server_ExecuteCrafting(ItemRowName);
	UE_LOG(LogTemp, Warning, TEXT("%s start to craft: "), *ItemRowName.ToString());
}
