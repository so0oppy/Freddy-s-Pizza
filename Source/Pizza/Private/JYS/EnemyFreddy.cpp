// Fill out your copyright notice in the Description page of Project Settings.


#include "JYS/EnemyFreddy.h"

// Sets default values
AEnemyFreddy::AEnemyFreddy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyFreddy::BeginPlay()
{
	Super::BeginPlay();
	
	SetAILevel(3);
}

// Called every frame
void AEnemyFreddy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

