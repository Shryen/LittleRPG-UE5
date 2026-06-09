#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LittleBaseCharacter.generated.h"

class ULittleStatComponent;

UCLASS()
class LITTLERPG_API ALittleBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALittleBaseCharacter();
	virtual void Tick(float DeltaTime) override;
	ULittleStatComponent* GetStatComponent() const {return LittleStatComponent;}

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TObjectPtr<ULittleStatComponent> LittleStatComponent;
	
	UFUNCTION()
	void HandleStatChanged(float NewStat);
};
