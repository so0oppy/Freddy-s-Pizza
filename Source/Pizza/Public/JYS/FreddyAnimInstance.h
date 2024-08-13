// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FreddyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UFreddyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// 초기화
	virtual void NativeInitializeAnimation() override;

	UPROPERTY()
	class USkeletalMeshComponent* Freddy;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Animation")
	class UAnimMontage* FreddyJumpscareMontage;

	UFUNCTION()
	void FreddyJumpscareAnimation();
};
