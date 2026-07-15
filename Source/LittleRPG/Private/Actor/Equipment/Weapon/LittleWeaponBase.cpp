#include "Actor/Equipment/Weapon/LittleWeaponBase.h"

#include "KismetTraceUtils.h"
#include "AbilitySystem/LittleAbilitySystemComponent.h"
#include "Character/LittleBaseCharacter.h"


ALittleWeaponBase::ALittleWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(GetRootComponent());
	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(GetRootComponent());
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetInstigator());
}

void ALittleWeaponBase::HitScan(TArray<FHitResult>& OutHit, bool& bWasHit)
{
	OutHit.Reset();

	TArray<FHitResult> SweepHits;

	FCollisionShape WeaponSphere = FCollisionShape::MakeSphere(HitScanRadius);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(GetInstigator());

	GetWorld()->SweepMultiByChannel(
		SweepHits,
		TraceStart->GetComponentLocation(),
		TraceEnd->GetComponentLocation(),
		FQuat::Identity,
		ECC_Visibility,
		WeaponSphere,
		Params
	);

	DrawDebugSweptSphere(
		GetWorld(),
		TraceStart->GetComponentLocation(),
		TraceEnd->GetComponentLocation(),
		HitScanRadius,
		FColor::Red,
		false,
		1.f
	);

	for (const FHitResult& Hit : SweepHits)
	{
		ALittleBaseCharacter* BaseCharacter = Cast<ALittleBaseCharacter>(Hit.GetActor());
		if (!BaseCharacter) continue;

		if (HitActors.Contains(BaseCharacter))
			continue;

		HitActors.AddUnique(BaseCharacter);
		OutHit.Add(Hit);
	}

	bWasHit = OutHit.Num() > 0;
}

void ALittleWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}


