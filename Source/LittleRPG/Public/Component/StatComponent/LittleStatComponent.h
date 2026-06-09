#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LittleStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChangedSignature, float, NewStat);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LITTLERPG_API ULittleStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULittleStatComponent();

	UFUNCTION(Server, Reliable)
	void ServerTakeDamage(float Damage);
	
	UPROPERTY(BlueprintAssignable)
	FOnStatChangedSignature OnStatChanged;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(ReplicatedUsing = OnRep_Health, EditDefaultsOnly)
	float Health{0};
	
	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, EditDefaultsOnly)
	float MaxHealth{0};
	
	UFUNCTION()
	void OnRep_Health() const;
	
	UFUNCTION()
	void OnRep_MaxHealth() const;
};
