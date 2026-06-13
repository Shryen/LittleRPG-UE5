#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LittleBaseCharacter.generated.h"

enum class EAttribute : uint8;
class ULittleStatComponent;

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateReadySignature)

UCLASS()
class LITTLERPG_API ALittleBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALittleBaseCharacter();
	virtual void Tick(float DeltaTime) override;
	ULittleStatComponent* GetStatComponent() const {return LittleStatComponent;}
	
	FOnPlayerStateReadySignature OnPlayerStateReady;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
private:
	UPROPERTY()
	TObjectPtr<ULittleStatComponent> LittleStatComponent;
	
	UFUNCTION()
	void HandleStatChanged(EAttribute StaType, float NewStat);
};
