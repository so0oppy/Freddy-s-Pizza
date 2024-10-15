// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NightmareMode.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UNightmareMode : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	bool bNightmare = false;
};
