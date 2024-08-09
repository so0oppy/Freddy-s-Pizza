// Fill out your copyright notice in the Description page of Project Settings.


#include "SB/FoxyAnimInstance.h"
#include "SB/Foxy.h"

void UFoxyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AFoxy* foxy = Cast<AFoxy>(TryGetPawnOwner());
	if ( nullptr == foxy )
	{
		return;
	}
}

void UFoxyAnimInstance::PlayJumpScareMontage()
{
	Montage_Play(FoxyJumpScare);
}

void UFoxyAnimInstance::PlayFakeScareMontage()
{
	FName SectionName = "Fake";
	Montage_JumpToSection(SectionName, FoxyFakeScare);
}
