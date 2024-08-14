// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DoorAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UDoorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	bool bClose = false;
	UPROPERTY(BlueprintReadWrite)
	bool bFoxyIn = false;
	UFUNCTION()
	void AnimNotify_FoxyIn();
};
