// Fill out your copyright notice in the Description page of Project Settings.


#include "SB/CupCake.h"
#include "Components/SkeletalMeshComponent.h"
#include "SB/CupCakeAnimInstance.h"

// Sets default values
ACupCake::ACupCake()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 컵케이크 mesh 설정
	CupcakeComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CupCakeComp"));
	CupcakeComp->SetupAttachment(RootComponent);
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

