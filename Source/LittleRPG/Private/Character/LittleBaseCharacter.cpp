#include "Character/LittleBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Component/LittleEquipManager.h"
#include "Components/CapsuleComponent.h"

ALittleBaseCharacter::ALittleBaseCharacter()
{
	bReplicates = true;
	SetReplicateMovement(true);
	
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
	//AbilitySystem
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(AscReplicationMode);
	
}

UAbilitySystemComponent* ALittleBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ALittleBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(AbilitySystemComponent, TEXT("AbilitySystemComponent is not valid."));
	
	if (!HasAuthority())
		return;
	

	if (!GivenAbilities.IsEmpty())
	    for (const TSubclassOf<UGameplayAbility>& AbilityClass : GivenAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
}

void ALittleBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	checkf(AbilitySystemComponent, TEXT("AbilitySystemComponent is not valid."));
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	OnPlayerStateReady.Broadcast();
}

void ALittleBaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	checkf(AbilitySystemComponent, TEXT("AbilitySystemComponent is not valid."));
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	OnPlayerStateReady.Broadcast();
}


void ALittleBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

