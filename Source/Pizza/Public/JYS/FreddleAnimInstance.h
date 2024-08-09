// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FreddleAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UFreddleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// 초기화
	virtual void NativeInitializeAnimation() override;

	void StartHideAnimation();

	UPROPERTY()
	class USkeletalMeshComponent* Freddle0;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Animation")
	class UAnimMontage* Freddle1Montage;

	UFUNCTION()
	void AnimNotify_HideStop();

	int32 LimitRepeatCount = 9999999;

	UPROPERTY()
	int32 Jumpscare1RepeatCount = 0;

private:

	UFUNCTION()
	void AnimNotify_Hide1End();


};
