// Fill out your copyright notice in the Description page of Project Settings.


#include "JYS/FreddyAnimInstance.h"
#include "JYS/EnemyFreddy.h"

void UFreddyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Freddy = GetOwningComponent();
}

void UFreddyAnimInstance::FreddyJumpscareAnimation()
{
	Montage_Play(FreddyJumpscareMontage);
}
