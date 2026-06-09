#include "GameModeBase/LittleGameModeBase.h"

#include "Character/LittlePlayerCharacter.h"
#include "Controller/PlayerController/LittlePlayerController.h"

ALittleGameModeBase::ALittleGameModeBase()
{
	PlayerControllerClass = ALittlePlayerController::StaticClass();
	DefaultPawnClass = ALittlePlayerCharacter::StaticClass();
}
