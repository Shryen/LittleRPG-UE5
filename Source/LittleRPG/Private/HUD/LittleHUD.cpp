#include "HUD/LittleHUD.h"
#include "Character/LittleBaseCharacter.h"
#include "Component/StatComponent/LittleStatComponent.h"
#include "HUD/Widget/LittleWidgetController.h"
#include "HUD/Widget/MainLayoutWidget.h"
#include "HUD/Widget/Inventory/InventoryWidget.h"
#include "HUD/Widget/Inventory/InventoryWidgetController.h"
#include "HUD/Widget/Stats/HealthBarWidgetController.h"
#include "HUD/Widget/Stats/HealthWidget.h"
#include "PlayerState/LittlePlayerState.h"

bool ALittleHUD::SetupMainOverlayWidget(APlayerController* PC)
{
	if (!MainOverlayWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALittleHUD::BeginPlay: MainOverlayWidget is not set. Set it in HUD!"));
		return false;
	}
	
	MainOverlayWidget = CreateWidget<UMainLayoutWidget>(PC, MainOverlayWidgetClass);
	if (MainOverlayWidget)
		MainOverlayWidget->AddToViewport();
	return true;
}

void ALittleHUD::SetupInventoryWidgetController(FWidgetControllerParams Params)
{
	InventoryWidgetController = NewObject<UInventoryWidgetController>(this);
	InventoryWidgetController->SetWidgetControllerParams(Params);
	InventoryWidgetController->SetWidget(MainOverlayWidget->InventoryWidget);
}

void ALittleHUD::SetupHealthBarWidgetController(FWidgetControllerParams Params)
{
	HealthBarWidgetController = NewObject<UHealthBarWidgetController>(this);
	HealthBarWidgetController->SetWidgetControllerParams(Params);
	HealthBarWidgetController->SetWidget(MainOverlayWidget->HealthBarWidget);
}

void ALittleHUD::SetupPlayerStateDependencies()
{
	ALittleBaseCharacter* Character = Cast<ALittleBaseCharacter>(GetOwningPawn());
	checkf(Character, TEXT("ALittleHUD::BeginPlay: Character not found"));
	
	Character->OnPlayerStateReady.AddUObject(this, &ALittleHUD::OnPlayerStateReady);

	if (Character->GetPlayerState())
	{
		OnPlayerStateReady();
	}
}

void ALittleHUD::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = GetOwningPlayerController();
	ALittlePlayerState* PS = PC->GetPlayerState<ALittlePlayerState>();
	ULittleStatComponent* LSC = nullptr;
	if (ALittleBaseCharacter* Character = Cast<ALittleBaseCharacter>(GetOwningPawn()))
		LSC = Character->GetStatComponent();

	FWidgetControllerParams Params(PC,PS,LSC);
	
	if (!SetupMainOverlayWidget(PC)) return;
	
	SetupInventoryWidgetController(Params);
	SetupHealthBarWidgetController(Params);
	
	MainOverlayWidget->InventoryWidget->SetWidgetController(InventoryWidgetController);
	MainOverlayWidget->HealthBarWidget->SetWidgetController(HealthBarWidgetController);
	
	SetupPlayerStateDependencies();
}

void ALittleHUD::OnPlayerStateReady()
{
	ALittleBaseCharacter* Character = Cast<ALittleBaseCharacter>(GetOwningPawn());
	checkf(Character, TEXT("ALittleHUD::OnPlayerState: Character not found"));
	HealthBarWidgetController->BindToStatComponent();
}
