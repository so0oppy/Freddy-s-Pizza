// Fill out your copyright notice in the Description page of Project Settings.


#include "JYS/AllEnemyCharacter.h"
#include "HJS/FreddyPlayer.h"

// Sets default values
AAllEnemyCharacter::AAllEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAllEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AFreddyPlayer* Temp = Cast<AFreddyPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Temp)
	{
		Player = Temp;
	}
}

// Called every frame
void AAllEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAllEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAllEnemyCharacter::SetAILevel(int32 Value)
{
	Level = Value;
}

//void AAllEnemyCharacter::JumpScare()
//{
//	
//}

