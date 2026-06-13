#include "HUD/LittleHUD.h"

#include "Character/LittleBaseCharacter.h"
#include "Component/StatComponent/LittleStatComponent.h"
#include "Data/ItemData.h"
#include "HUD/Widget/LittleWidgetController.h"
#include "HUD/Widget/MainLayoutWidget.h"
#include "HUD/Widget/Inventory/InventoryWidget.h"
#include "HUD/Widget/Inventory/InventoryWidgetController.h"
#include "HUD/Widget/Stats/HealthWidget.h"
#include "PlayerState/LittlePlayerState.h"

void ALittleHUD::BeginPlay()
{
	Super::BeginPlay();
	if (!HealthBarWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALittleHUD::BeginPlay: A widget is not set! Set it in HUD!"));
		return;
	}
	
	if (!MainOverlayWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALittleHUD::BeginPlay: MainOverlayWidget is not set. Set it in HUD!"));
		return;
	}
	
	APlayerController* PC = GetOwningPlayerController();
	APlayerState* PS = PC->GetPlayerState<APlayerState>();
	ULittleStatComponent* LSC = nullptr;
	if (ALittleBaseCharacter* Character = Cast<ALittleBaseCharacter>(GetOwningPawn()))
		LSC = Character->GetStatComponent();

	FWidgetControllerParams Params(PC,PS,LSC);
	
	MainOverlayWidget = CreateWidget<UMainLayoutWidget>(PC, MainOverlayWidgetClass);
	if (MainOverlayWidget)
		MainOverlayWidget->AddToViewport();
	
	InventoryWidgetController = NewObject<UInventoryWidgetController>(this);
	InventoryWidgetController->SetWidgetControllerParams(Params);
	InventoryWidgetController->SetWidget(MainOverlayWidget->InventoryWidget);

	
	MainOverlayWidget->InventoryWidget->SetWidgetController(InventoryWidgetController);
	
	
	HealthBarWidget = CreateWidget<UHealthWidget>(PC, HealthBarWidgetClass);
	HealthBarWidget->SetVisibility(ESlateVisibility::Collapsed);
	//HealthBarWidget->AddToViewport();
	
	ALittleBaseCharacter* Character = Cast<ALittleBaseCharacter>(GetOwningPawn());
	checkf(Character, TEXT("ALittleHUD::BeginPlay: Character not found"));
	
	Character->OnPlayerStateReady.AddUObject(this, &ALittleHUD::OnPlayerStateReady);

	if (Character->GetPlayerState())
	{
		OnPlayerStateReady();
	}
}

void ALittleHUD::OnPlayerStateReady()
{
	ALittleBaseCharacter* Character = Cast<ALittleBaseCharacter>(GetOwningPawn());
	checkf(Character, TEXT("ALittleHUD::OnPlayerState: Character not found"));
	ALittlePlayerState* PlayerState = Cast<ALittlePlayerState>(Character->GetPlayerState());
	checkf(PlayerState, TEXT("ALittleHUD::OnPlayerState:  PlayerState null after OnPlayerStateReady"));
	
	InventoryWidgetController->BindPlayerStateToInventory(PlayerState);
	
	ULittleStatComponent* StatComp = Character->GetStatComponent();
	if (StatComp)
		StatComp->OnHealthChanged.AddDynamic(this, &ALittleHUD::OnHealthChanged);
	
	HealthBarWidget->UpdateHealth(
		Character->GetStatComponent()->GetCurrentHealth(), 
		Character->GetStatComponent()->GetMaxHealth());
}

void ALittleHUD::OnHealthChanged(const float CurrentHealth)
{
	if (!HealthBarWidget) return;
	HealthBarWidget->UpdateHealth(CurrentHealth, 100);
}

