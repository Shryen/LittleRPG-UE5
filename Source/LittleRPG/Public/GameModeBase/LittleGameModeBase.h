#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LittleGameModeBase.generated.h"

class ALittlePlayerState;
/**
 * 
 */
UCLASS()
class LITTLERPG_API ALittleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALittleGameModeBase();
	void SaveGame() const;
	void LoadGame(ALittlePlayerState* PlayerState) const;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void OnPostLogin(AController* NewPlayer) override;

	
private:
	const FString SaveSlotName = TEXT("Save1");
	const int32 UserIndex = 0;
};
