// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LocationState.generated.h"


UENUM(BlueprintType)
enum class ELocationState : uint8 
{
	Idle,
	Move,
	Attack
};
