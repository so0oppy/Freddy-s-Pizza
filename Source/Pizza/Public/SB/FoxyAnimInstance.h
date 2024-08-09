// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FoxyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UFoxyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* FoxyJumpScare;
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* FoxyFakeScare;

	UFUNCTION(BlueprintCallable)
	void PlayJumpScareMontage();
	UFUNCTION(BlueprintCallable)
	void PlayFakeScareMontage();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsJumpScare;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsFakeScare;
};
