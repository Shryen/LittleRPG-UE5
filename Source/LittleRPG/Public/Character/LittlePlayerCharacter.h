#pragma once

#include "CoreMinimal.h"
#include "LittleBaseCharacter.h"
#include "LittlePlayerCharacter.generated.h"

UCLASS()
class LITTLERPG_API ALittlePlayerCharacter : public ALittleBaseCharacter
{
	GENERATED_BODY()

public:
	ALittlePlayerCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

};
