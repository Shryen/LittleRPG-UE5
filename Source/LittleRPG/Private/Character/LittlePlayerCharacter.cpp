#include "Character/LittlePlayerCharacter.h"



ALittlePlayerCharacter::ALittlePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALittlePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALittlePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

