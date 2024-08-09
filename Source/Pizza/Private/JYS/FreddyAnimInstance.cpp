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
	//UE_LOG(LogTemp,Warning,TEXT("1111111"));
	//AEnemyFreddy* Me = Cast<AEnemyFreddy>(TryGetPawnOwner());
	Montage_Play(FreddyJumpscareMontage);
	//Me->GetMesh()->GetAnimInstance()->Montage_Play(FreddyJumpscareMontage);
}
