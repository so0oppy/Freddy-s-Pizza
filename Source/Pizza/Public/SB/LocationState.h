#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LocationState.generated.h"


UENUM(BlueprintType)
enum class ELocationState : uint8 
{
	IDLE,
	MOVE,
	ATTACK,
	CUPCAKE,
	CLOSET
};
