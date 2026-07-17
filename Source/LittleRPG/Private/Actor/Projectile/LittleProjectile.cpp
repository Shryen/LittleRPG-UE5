#include "Actor/Projectile/LittleProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "NiagaraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


ALittleProjectile::ALittleProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	HitCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollision"));
	HitCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	HitCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	HitCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	SetRootComponent(HitCollision);
	
	Projectile = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Projectile"));
	Projectile->SetupAttachment(GetRootComponent());
	
	ProjectileDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileDirection"));
	ProjectileDirection->SetupAttachment(GetRootComponent());
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bAutoActivate = false;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void ALittleProjectile::InitProjectileMovementComponent()
{
	FVector Direction = TargetLocation - GetActorLocation();
	FVector UnitDirection = Direction.GetSafeNormal();
	
	FVector Velocity = UnitDirection*ProjectileSpeed;
	
	ProjectileMovementComponent->Velocity = Velocity;
	
	ProjectileMovementComponent->Activate();
}

void ALittleProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void ALittleProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetInstigator())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor))
		{
			FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(ProjectileEffect, 1, ContextHandle);
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			Destroy();
		} else
			Destroy();
		
	}	
}


void ALittleProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	
	InitProjectileMovementComponent();
	
	HitCollision->OnComponentHit.AddDynamic(this, &ALittleProjectile::OnHit);
	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &ALittleProjectile::OnOverlapBegin);
}

