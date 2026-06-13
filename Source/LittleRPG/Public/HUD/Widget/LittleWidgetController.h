#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LittleWidgetController.generated.h"

class ULittleUserWidget;
class ULittleStatComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	
	FWidgetControllerParams(){};
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, ULittleStatComponent* LSC)
		: PlayerController(PC), PlayerState(PS), StatComponent(LSC){};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ULittleStatComponent> StatComponent = nullptr;
};
/**
 * 
 */
UCLASS()
class LITTLERPG_API ULittleWidgetController : public UObject	
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& Params);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetWidget(ULittleUserWidget* InWidget);
protected:
	UPROPERTY(BlueprintReadOnly, Category = "LittleRPG|Widget|Controller")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category = "LittleRPG|Widget|Controller")
	TObjectPtr<APlayerState> PlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category = "LittleRPG|Widget|Controller")
	TObjectPtr<ULittleStatComponent> StatComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULittleUserWidget> OwnerWidget;
};
