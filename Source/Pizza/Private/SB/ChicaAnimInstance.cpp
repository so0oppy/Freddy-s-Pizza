// Fill out your copyright notice in the Description page of Project Settings.


#include "SB/ChicaAnimInstance.h"
#include "SB/Chica.h"

void UChicaAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AChica* chica = Cast<AChica>(TryGetPawnOwner());
	if ( nullptr == chica )
	{
		return;
	}
}

void UChicaAnimInstance::PlayJumpScareMontage()
{
	Montage_Play(ChicaJumpScare);
}
