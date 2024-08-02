// Fill out your copyright notice in the Description page of Project Settings.


#include "SB/FoxDoll.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFoxDoll::AFoxDoll()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 폭시 인형 (기존 위치에 두고 안 보이다가 보이게) -> 따로 생성
 	FoxyDollComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoxyDollComp"));
 	FoxyDollComp->SetupAttachment(RootComponent);
 	FoxyDollComp->SetRelativeLocation(FVector(120.f, 700.f, 130.f));  // 옷장 뒤에 투명 물체 두고 위치 따와서 여기 넣기
 
}

// Called when the game starts or when spawned
void AFoxDoll::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFoxDoll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

