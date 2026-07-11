

#include "Actor/EffectArea/EffectAreaBase.h"

#include "AbilitySystem/GameplayTags/LittleGameplayTags.h"
#include "Character/LittleBaseCharacter.h"
#include "Components/BoxComponent.h"


AEffectAreaBase::AEffectAreaBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	SetRootComponent(Collision);
	Magnitude = -25.f;
	GameplayTag = TAG_Data_Damage;
}

void AEffectAreaBase::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		ALittleBaseCharacter* BaseCharacter = Cast<ALittleBaseCharacter>(OtherActor);
		if (!BaseCharacter) return;
		
		UAbilitySystemComponent* TargetASC = BaseCharacter->GetAbilitySystemComponent();
		if (!TargetASC) return;
		
		FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
		Context.AddSourceObject(this);
		SpecHandle = TargetASC->MakeOutgoingSpec(Effect, 1, Context);
		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(GameplayTag, Magnitude);
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AEffectAreaBase::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		ALittleBaseCharacter* BaseCharacter = Cast<ALittleBaseCharacter>(OtherActor);
		if (!BaseCharacter) return;
		
		UAbilitySystemComponent* TargetASC = BaseCharacter->GetAbilitySystemComponent();
		if (!TargetASC) return;
		
		TargetASC->RemoveActiveGameplayEffectBySourceEffect(Effect, TargetASC);
	}
}

void AEffectAreaBase::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AEffectAreaBase::OverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AEffectAreaBase::OverlapEnd);
}

