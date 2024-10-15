// Fill out your copyright notice in the Description page of Project Settings.


#include "SB/FoxDoll.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFoxDoll::AFoxDoll()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ���� ���� (���� ��ġ�� �ΰ� �� ���̴ٰ� ���̰�) -> ���� ����
 	FoxyDollComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoxyDollComp"));
 	FoxyDollComp->SetupAttachment(RootComponent);
 	FoxyDollComp->SetRelativeLocation(FVector(120.f, 700.f, 130.f));  // ���� �ڿ� ���� ��ü �ΰ� ��ġ ���ͼ� ���� �ֱ�
 
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

