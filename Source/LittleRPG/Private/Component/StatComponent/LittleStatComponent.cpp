#include "Component/StatComponent/LittleStatComponent.h"

#include "Net/UnrealNetwork.h"


ULittleStatComponent::ULittleStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	
	MaxHealth = 100.0f;
	Health = MaxHealth;
}

void ULittleStatComponent::ServerTakeDamage_Implementation(float Damage)
{
	Health = FMath::Max(0.f, Health - Damage);
	OnHealthChanged.Broadcast(Health);
}

void ULittleStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULittleStatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ULittleStatComponent, Health);
	DOREPLIFETIME(ULittleStatComponent, MaxHealth);
}


void ULittleStatComponent::OnRep_Health() const
{
	OnHealthChanged.Broadcast(Health);
}


void ULittleStatComponent::OnRep_MaxHealth() const
{
	OnHealthChanged.Broadcast(MaxHealth);
}

