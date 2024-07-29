// Fill out your copyright notice in the Description page of Project Settings.


#include "SB/Chica.h"
#include "SB/LocationState.h"
#include "Chica.h"

// Sets default values
AChica::AChica()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = ELocationState::Idle;

}

// Called when the game starts or when spawned
void AChica::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChica::SetUpLocation(ELocationState State)
{
	switch (State)
	{
	case ELocationState::Idle:
		break;
	case ELocationState::Move:
		break;
	case ELocationState::Attack:
		break;
	default:
		break;
	}
}

// Called every frame
void AChica::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChica::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



