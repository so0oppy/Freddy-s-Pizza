// Fill out your copyright notice in the Description page of Project Settings.


#include "JYS/FreddleAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "JYS/EnemyFreddy.h"

void UFreddleAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//Freddle0 = Cast<AEnemyFreddy>(GetOwningComponent())->FreddyMesh0;
	Freddle0 = GetOwningComponent();
}

void UFreddleAnimInstance::StartHideAnimation()
{

	Montage_JumpToSection(FName("Hide1") , Freddle1Montage);
	LimitRepeatCount = 3;
	Jumpscare1RepeatCount = 0;
}

void UFreddleAnimInstance::AnimNotify_HideStop()
{
	Freddle0 = GetOwningComponent();
	
	Freddle0->SetHiddenInGame(true);
	Montage_JumpToSection(FName("Hide1") , Freddle1Montage);
	LimitRepeatCount = 9999999;
	Jumpscare1RepeatCount = 0;
}

void UFreddleAnimInstance::AnimNotify_Hide1End()
{
	Jumpscare1RepeatCount++;
	if ( Jumpscare1RepeatCount >= LimitRepeatCount )
	{
		Montage_JumpToSection(FName("Hide2") , Freddle1Montage);
		Jumpscare1RepeatCount = 0;

	}
}


