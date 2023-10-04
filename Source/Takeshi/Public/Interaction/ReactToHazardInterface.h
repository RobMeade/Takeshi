// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "UObject/Interface.h"
#include "CoreMinimal.h"

#include "ReactToHazardInterface.generated.h"


UINTERFACE(MinimalAPI)
class UReactToHazardInterface : public UInterface
{
	GENERATED_BODY()
};


class TAKESHI_API IReactToHazardInterface
{
	GENERATED_BODY()


public:

	virtual void ReactToHazard() = 0;

};
