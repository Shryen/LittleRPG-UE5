#include "Controller/PlayerController/LittlePlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "AbilitySystem/GameplayTags/LittleGameplayTags.h"
#include "Actor/Interactable/InteractableObject.h"
#include "Actor/Resource/ResourceNode.h"
#include "Character/LittleBaseCharacter.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "HUD/LittleHUD.h"
#include "HUD/Widget/MainLayoutWidgetController.h"
#include "PlayerState/LittlePlayerState.h"
#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "GameFramework/Character.h"

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

	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALittlePlayerController::Move);
	EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered,this, &ALittlePlayerController::Look);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started,this, &ALittlePlayerController::Jump);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed,this, &ALittlePlayerController::StopJump);
	EnhancedInput->BindAction(InventoryAction, ETriggerEvent::Triggered,this, &ALittlePlayerController::HandleInventory);
	EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &ALittlePlayerController::HandleInteract);
	EnhancedInput->BindAction(AttackAction, ETriggerEvent::Started, this, &ALittlePlayerController::Attack);
	
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

void ALittlePlayerController::Jump(const FInputActionValue& Value)
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->Jump();
	}
}

void ALittlePlayerController::StopJump(const FInputActionValue& Value)
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->StopJumping();
	}
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

void ALittlePlayerController::Attack(const FInputActionValue& Value)
{
	ALittleBaseCharacter* BaseCharacter = Cast<ALittleBaseCharacter>(GetPawn());
	if (!BaseCharacter) return;
	FGameplayEventData EventData;
	EventData.EventTag = TAG_Event_ContinueCombo_Input;
	EventData.Instigator = BaseCharacter;
	
	const FGameplayTagContainer AttackTag =
	FGameplayTagContainer(BaseCharacter->GetAutoAttackTag());
	
	UE_LOG(LogTemp, Warning, TEXT("AttackTag used in PlayerController: %s"), *AttackTag.ToString())
	
	BaseCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(AttackTag);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(BaseCharacter, EventData.EventTag, EventData);
	if (!HasAuthority())
		BaseCharacter->Server_SendGameplayEventToSelf(EventData);
}

void ALittlePlayerController::CheatAddItem(FName ItemRowName, int32 Quantity)
{
	if (!HasAuthority()) return;
	ALittlePlayerState* PS = GetPlayerState<ALittlePlayerState>();
	if (!PS) return;
	ULittleInventoryManagerComponent* Inv = PS->GetInventoryManager();
	if (!Inv) return;
	Inv->AddItemToInventory(ItemRowName, Quantity);
}

