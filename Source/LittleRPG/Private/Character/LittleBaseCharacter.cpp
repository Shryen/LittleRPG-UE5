#include "Character/LittleBaseCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/LittleAbilitySystemComponent.h"
#include "AbilitySystem/GameplayTags/LittleGameplayTags.h"
#include "AbilitySystem/AttributeSet/LittleAttributeSet.h"
#include "Component/Equipment/LittleEquipManager.h"
#include "Components/CapsuleComponent.h"

ALittleBaseCharacter::ALittleBaseCharacter()
{
	bReplicates = true;
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
	AbilitySystemComponent = CreateDefaultSubobject<ULittleAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(AscReplicationMode);
	
	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);
	
	LittleAttributeSet = CreateDefaultSubobject<ULittleAttributeSet>(TEXT("LittleAttributeSet"));
}

UAbilitySystemComponent* ALittleBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ALittleBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	checkf(AbilitySystemComponent, TEXT("AbilitySystemComponent is not valid."));
}


TArray<FGameplayAbilitySpecHandle> ALittleBaseCharacter::GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant)
{
	if (!AbilitySystemComponent || !HasAuthority()) return TArray<FGameplayAbilitySpecHandle>();
	
	TArray<FGameplayAbilitySpecHandle> AbilityHandles;
	
	for (const TSubclassOf<UGameplayAbility>& Ability : AbilitiesToGrant)
	{
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(Ability, 1, 
			-1, this));
		AbilityHandles.Add(SpecHandle);
	}
	
	SendAbilitiesChangedEvent();
	
	return AbilityHandles;
}

void ALittleBaseCharacter::RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove)
{
	if (!AbilitySystemComponent || !HasAuthority()) return;
	
	for (const FGameplayAbilitySpecHandle& AbilityHandle : AbilitiesToRemove)
	{
		AbilitySystemComponent->ClearAbility(AbilityHandle);
	}
	
	SendAbilitiesChangedEvent();
}

void ALittleBaseCharacter::SendAbilitiesChangedEvent()
{
	FGameplayEventData EventData;
	EventData.EventTag = TAG_Event_Abilities_Changed;
	EventData.Instigator = this;
	EventData.Target = this;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData);
}


void ALittleBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	checkf(AbilitySystemComponent, TEXT("AbilitySystemComponent is not valid."));
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitDefaultGameplayEffects();
	
	if (!StartingAbilities.IsEmpty())
		GrantAbilities(StartingAbilities);
	else
		UE_LOG(LogTemp, Warning, TEXT("ALittleBaseCharacter::PosessedBy: No abilities given to character: [%s]"), *GetName());
	
	BindDependencies();
	OnPlayerStateReady.Broadcast();
}

void ALittleBaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	checkf(AbilitySystemComponent, TEXT("AbilitySystemComponent is not valid."));
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitDefaultGameplayEffects();
	BindDependencies();
	OnPlayerStateReady.Broadcast();
}


void ALittleBaseCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (!HasAuthority()) return; 

	if (Data.NewValue < Data.OldValue)
	{
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(
	   FGameplayTagContainer(TAG_Regen_Health));
		
		FGameplayEffectContextHandle Handle = AbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle DelaySpec = AbilitySystemComponent->MakeOutgoingSpec(
			GE_HealthRegen, 1, Handle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DelaySpec.Data.Get());
	} else if (Data.NewValue >= LittleAttributeSet->GetMaxHealth())
	{
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(
			FGameplayTagContainer(TAG_Regen_Health));
	}
}

void ALittleBaseCharacter::BindDependencies()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	   ULittleAttributeSet::GetStaminaAttribute())
	   .AddUObject(this, &ALittleBaseCharacter::OnStaminaChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
   ULittleAttributeSet::GetHealthAttribute())
   .AddUObject(this, &ALittleBaseCharacter::OnHealthChanged);
}

void ALittleBaseCharacter::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (!HasAuthority()) return;

	if (Data.NewValue < Data.OldValue)
	{
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(
		   FGameplayTagContainer(TAG_Regen_Stamina));
		
		FGameplayEffectContextHandle Handle = AbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle DelaySpec = AbilitySystemComponent->MakeOutgoingSpec(
			GE_StaminaRegen, 1, Handle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DelaySpec.Data.Get());
	}    else if (Data.NewValue >= LittleAttributeSet->GetMaxStamina())
	{
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(
			FGameplayTagContainer(TAG_Regen_Stamina));
	}
}

void ALittleBaseCharacter::InitDefaultGameplayEffects()
{
	if (!HasAuthority())
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle MainSpecHandle =
		AbilitySystemComponent->MakeOutgoingSpec(
			InitialGameplayEffects,
			1.f,
			EffectContext);

	if (MainSpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*MainSpecHandle.Data.Get());
	}
	
	FGameplayEffectSpecHandle SecondarySpecHandle =
		AbilitySystemComponent->MakeOutgoingSpec(
			InitialSecondaryGameplayEffects,
			1.f,
			EffectContext);
	
	if (SecondarySpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());
	}
	
	LittleAttributeSet->InitHealth(LittleAttributeSet->GetMaxHealth());
	LittleAttributeSet->InitStamina(LittleAttributeSet->GetMaxStamina());
	
}

void ALittleBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}