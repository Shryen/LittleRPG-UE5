#include "Actor/Equipment/Weapon/LittleWeaponBase.h"


ALittleWeaponBase::ALittleWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());
}

void ALittleWeaponBase::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor!=this)
	{
		if (bool bFound = ActorsToIgnore.Contains(OtherActor)) return;
		
		
	}
}

void ALittleWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}


