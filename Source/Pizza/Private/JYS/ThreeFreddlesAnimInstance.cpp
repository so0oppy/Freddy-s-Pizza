// Fill out your copyright notice in the Description page of Project Settings.


#include "JYS/ThreeFreddlesAnimInstance.h"
#include "JYS/EnemyFreddy.h"

void UThreeFreddlesAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Freddle0 = GetOwningComponent();
}

void UThreeFreddlesAnimInstance::StartHideAnimation(int32 FreddleNum)
{
	FString HideNum = FString::Printf(TEXT("Hide%d"), FreddleNum);
	Montage_JumpToSection(FName(HideNum) , Freddle1Montage);
}

void UThreeFreddlesAnimInstance::StartIdleAnimation(int32 FreddleNum)
{
	FString IdleNum = FString::Printf(TEXT("Idle%d") , FreddleNum);
	Montage_JumpToSection(FName(IdleNum) , Freddle1Montage);
}

void UThreeFreddlesAnimInstance::AnimNotify_HideStop()
{
	Freddle0 = GetOwningComponent();

	Freddle0->SetHiddenInGame(true);
}
