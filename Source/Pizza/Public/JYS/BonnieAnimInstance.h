// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BonnieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UBonnieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// 초기화
	virtual void NativeInitializeAnimation() override;

	UPROPERTY()
	class USkeletalMeshComponent* Bonnie;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Animation")
	class UAnimMontage* BonnieJumpscareMontage;

	UFUNCTION()
	void BonnieJumpscareAnimation();
};
