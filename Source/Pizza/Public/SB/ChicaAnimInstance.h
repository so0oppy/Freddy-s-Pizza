// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ChicaAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UChicaAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ChicaJumpScare;

	UFUNCTION(BlueprintCallable)
	void PlayJumpScareMontage();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsJumpScare;
};
