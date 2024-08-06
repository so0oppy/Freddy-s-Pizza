// Fill out your copyright notice in the Description page of Project Settings.


#include "SB/CupCake.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACupCake::ACupCake()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 임시 컵케이크
	CupCakeComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CupCakeComp"));
	CupCakeComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACupCake::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACupCake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

