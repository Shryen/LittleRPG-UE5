#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LITTLERPG_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(AActor* Interactor) = 0;
	virtual FText GetInteractText() = 0;
};
