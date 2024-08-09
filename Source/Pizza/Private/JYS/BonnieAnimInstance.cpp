// Fill out your copyright notice in the Description page of Project Settings.


#include "JYS/BonnieAnimInstance.h"

void UBonnieAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Bonnie = GetOwningComponent();
}

void UBonnieAnimInstance::BonnieJumpscareAnimation()
{
	Montage_Play(BonnieJumpscareMontage);
}
