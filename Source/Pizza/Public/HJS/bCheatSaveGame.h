// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "bCheatSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UbCheatSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnyWhere)
	bool bHouseMap = false;
	UPROPERTY(EditAnyWhere)
	bool bFastNight = false;
};
