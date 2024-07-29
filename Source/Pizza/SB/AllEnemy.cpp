// Fill out your copyright notice in the Description page of Project Settings.


#include "AllEnemy.h"
#include "LocationState.h"

// Sets default values
AAllEnemy::AAllEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AAllEnemy::SetUpLocation(ELocationState State)
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

// Called when the game starts or when spawned
void AAllEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAllEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAllEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

