// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CupCakeAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UCupCakeAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* CupCakeJumpScare;

	UFUNCTION(BlueprintCallable)
	void PlayJumpScareMontage();
};
