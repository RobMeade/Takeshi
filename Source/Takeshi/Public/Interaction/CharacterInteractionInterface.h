// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "UObject/Interface.h"
#include "CoreMinimal.h"

#include "CharacterInteractionInterface.generated.h"


// Forward Declarations
class ATakeshiCharacterBase;


UINTERFACE(MinimalAPI)
class UCharacterInteractionInterface : public UInterface
{
	GENERATED_BODY()
};


class TAKESHI_API ICharacterInteractionInterface
{
	GENERATED_BODY()


public:

	virtual void CharacterInteractionBegin(ATakeshiCharacterBase* InteractingCharacter) = 0;
	virtual void CharacterInteractionEnd(ATakeshiCharacterBase* InteractingCharacter) = 0;

};
