#include "Controller/PlayerController/LittlePlayerController.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Actor/Interactable/InteractableObject.h"
#include "Actor/Resource/ResourceNode.h"
#include "Character/LittlePlayerCharacter.h"
#include "Component/StatComponent/LittleStatComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "HUD/LittleHUD.h"
#include "HUD/Widget/MainLayoutWidgetController.h"

ALittlePlayerController::ALittlePlayerController()
{
}

void ALittlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(MovementMappingContext, 0);
		}
	}
	
	FActorSpawnParameters Params;
	Params.bNoFail = true;
	for (auto& Pair : ResourceNodeClassMap)
	{
		if (Pair.Value)
		{
			AResourceNode* Warmup = GetWorld()->SpawnActor<AResourceNode>(
				Pair.Value, FTransform::Identity, Params);
			if (Warmup) Warmup->Destroy();
			UE_LOG(LogTemp, Warning, TEXT("Warmup for object: %s"), *Pair.Value->GetName());
		}
	}
}

void ALittlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInput =
	  Cast<UEnhancedInputComponent>(InputComponent);

	if (!EnhancedInput)
	{
		return;
	}

	EnhancedInput->BindAction(
		MoveAction,
		ETriggerEvent::Triggered,
		this,
		&ALittlePlayerController::Move);

	EnhancedInput->BindAction(
		LookAction,
		ETriggerEvent::Triggered,
		this,
		&ALittlePlayerController::Look);
	
	EnhancedInput->BindAction(
	TakeDamageTest,
	ETriggerEvent::Triggered,
	this,
	&ALittlePlayerController::TestDamage);
	
	EnhancedInput->BindAction(
		InventoryAction,
		ETriggerEvent::Triggered,
		this,
		&ALittlePlayerController::HandleInventory);
	
	EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &ALittlePlayerController::HandleInteract);
}

void ALittlePlayerController::Server_InteractAndSwap_Implementation(UInstancedStaticMeshComponent* Component,
	int32 InstanceIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("[Server_InteractAndSwap] Called. Component=%s InstanceIndex=%d HasAuthority=%d"),
		Component ? *Component->GetFName().ToString() : TEXT("NULL"), InstanceIndex, HasAuthority());

	if (!Component || !HasAuthority()) return;
	if (!Component->IsValidInstance(InstanceIndex)) return;
	
	TSubclassOf<AResourceNode> SpawnClass = nullptr;
	for (const FName& TagName : Component->ComponentTags)
	{
		if (!UGameplayTagsManager::Get().IsValidGameplayTagString(TagName.ToString()))
			continue;

		FGameplayTag Tag =
			FGameplayTag::RequestGameplayTag(TagName, false);

		if (!Tag.IsValid()) continue;
		
		if (const TSubclassOf<AResourceNode>* Found = ResourceNodeClassMap.Find(Tag))
		{
			SpawnClass = *Found;
			break;
		}
		
	}
	
	if (!SpawnClass) return;
	
	FTransform InstanceTransform;
	Component->GetInstanceTransform(InstanceIndex, InstanceTransform, true);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	

	if (AResourceNode* Node = GetWorld()->SpawnActor<AResourceNode>(SpawnClass, InstanceTransform, SpawnParams))
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server_InteractAndSwap] Spawned ResourceNode, calling SetDestroyedISMCInfo"));
		Node->SetDestroyedISMCInfo(Component, InstanceIndex);
		Node->Interact(GetPawn());
	}
}

void ALittlePlayerController::Server_Interact_Implementation(AActor* InteractableActor)
{
	if (!InteractableActor) return;

	IInteractable* Interactable = Cast<IInteractable>(InteractableActor);
	if (!Interactable) return;

	Interactable->Interact(GetPawn());
}

void ALittlePlayerController::Move(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

		const FVector2D MovementVector = Value.Get<FVector2D>();

		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDirection =
			FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection =
			FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
}

void ALittlePlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(-LookAxisVector.Y);
}

void ALittlePlayerController::HandleInventory(const FInputActionValue& Value)
{
	ALittleHUD* LittleHUD = Cast<ALittleHUD>(GetHUD());
	if (!LittleHUD)
		return;
	
	if (UMainLayoutWidgetController* MainController = LittleHUD->GetMainLayoutWidgetController())
		MainController->ToggleInventory();
}

void ALittlePlayerController::HandleInteract(const FInputActionValue& Value)
{
	// ALittleHUD* LittleHUD = Cast<ALittleHUD>(GetHUD());
	//if (LittleHUD->GetInventoryWidgetController()->IsOpen())
	//	return;
	FVector CameraLocation;
	FRotator CameraRotation;
	GetPlayerViewPoint(CameraLocation, CameraRotation);
	
	FVector TraceEnd = CameraLocation + CameraRotation.Vector() * 500.f;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPawn());
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit, CameraLocation, TraceEnd,
		ECC_Visibility, Params);
	
	DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, bHit ? FColor::Green : FColor::Red, false, 2.f);
	
	if (!bHit) return;
	
	if (UInstancedStaticMeshComponent* ISMC = Cast<UInstancedStaticMeshComponent>(Hit.Component))
	{
		for (const FName& TagName : ISMC->ComponentTags)
		{
			FGameplayTag Tag = FGameplayTag::RequestGameplayTag(TagName, false);
			UE_LOG(LogTemp, Warning, TEXT("TagName: %s"), *TagName.ToString());
			UE_LOG(LogTemp, Warning, TEXT("Tag: %s"), *Tag.ToString());
			if (!Tag.IsValid()) continue;
			if (Tag.IsValid() && ResourceNodeClassMap.Contains(Tag))
			{
				Server_InteractAndSwap(ISMC, Hit.Item);
				return;
			}
		}
	}
	Server_Interact(Hit.GetActor());
}

void ALittlePlayerController::TestDamage()
{
		ALittlePlayerCharacter* CharacterPawn = Cast<ALittlePlayerCharacter>(GetPawn());
		if (CharacterPawn)
		{
			CharacterPawn->GetStatComponent()->ServerTakeDamage(10);
		}
}


