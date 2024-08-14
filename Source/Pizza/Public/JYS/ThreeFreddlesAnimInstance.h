// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ThreeFreddlesAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UThreeFreddlesAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// 초기화
	virtual void NativeInitializeAnimation() override;

	void StartHideAnimation(int32 FreddleNum);
	void StartIdleAnimation(int32 FreddleNum);

	UPROPERTY()
	class USkeletalMeshComponent* Freddle0;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Animation")
	class UAnimMontage* Freddle1Montage;

	UFUNCTION()
	void AnimNotify_HideStop();
};
