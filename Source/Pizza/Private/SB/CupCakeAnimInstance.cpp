// Fill out your copyright notice in the Description page of Project Settings.


#include "SB/CupCakeAnimInstance.h"
#include "SB/CupCake.h"

void UCupCakeAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	
	ACupCake* cupcake = Cast<ACupCake>(TryGetPawnOwner());
	if ( nullptr == cupcake )
	{
		return;
	}
}

void UCupCakeAnimInstance::PlayJumpScareMontage()
{
	Montage_Play(CupCakeJumpScare);
}
