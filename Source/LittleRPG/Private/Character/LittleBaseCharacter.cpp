#include "Character/LittleBaseCharacter.h"

#include "Component/LittleEquipManager.h"
#include "Component/StatComponent/LittleStatComponent.h"
#include "Components/CapsuleComponent.h"

ALittleBaseCharacter::ALittleBaseCharacter()
{
	bReplicates = true;
	SetReplicateMovement(true);
	
	LittleStatComponent = CreateDefaultSubobject<ULittleStatComponent>(TEXT("LittleStatComponent"));
	LightMagicSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LightMagicSceneComponent"));
	LightMagicSceneComponent->SetupAttachment(GetRootComponent());
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	
	// GameTraceChannel1 = Pickup objects
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	
	EquipManager = CreateDefaultSubobject<ULittleEquipManager>(TEXT("EquipManagerComponent"));
}

void ALittleBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	LittleStatComponent->OnStatChanged.AddDynamic(this, &ALittleBaseCharacter::HandleStatChanged);
}

void ALittleBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	OnPlayerStateReady.Broadcast();
}

void ALittleBaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnPlayerStateReady.Broadcast();
}

void ALittleBaseCharacter::HandleStatChanged(EAttribute StatType, float NewStat)
{
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), NewStat);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
		
		if (NewStat <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}
	
    if (GetLocalRole() == ROLE_Authority)	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), NewStat);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}
}

void ALittleBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

