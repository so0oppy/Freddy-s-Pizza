// Fill out your copyright notice in the Description page of Project Settings.


#include "JYS/EnemyFreddy.h"
#include "HJS/FreddyPlayer.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

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

	// 큐브(Freddy) 스폰 타이머
	GetWorld()->GetTimerManager().SetTimer(CubeSpawnTimerHandle, this, &AEnemyFreddy::AttemptSpawnCube, 3.02f, true);

	// Player 객체 찾기
	for (TActorIterator<AFreddyPlayer> It(GetWorld()); It; ++It)
	{
		Player = *It;
		break;
	}
}

// Called every frame
void AEnemyFreddy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPlayerLookingAtBedAndFlashOn() && !GetWorld()->GetTimerManager().IsTimerActive(ShrinkTimerHandle))
	{
		// 딜레이가 끝난 후 큐브가 작아지기
		GetWorld()->GetTimerManager().SetTimer(ShrinkTimerHandle, this, &AEnemyFreddy::StartShrinkingCubes, SHRINK_DELAY, false);  
	}
}

void AEnemyFreddy::AttemptSpawnCube()
{
	// 만약 플레이어가 침대를 바라보고 있지 않다면
	if (!IsPlayerLookingAtBedAndFlashOn())
	{
		// 랜덤 넘버를 고른다
		int32 RandomNumber = GetRandomNumber();

		// 랜덤 넘버가 Freddy의 레벨보다 낮거나 같다면
		if (RandomNumber <= Level)
		{
			// 큐브(Freddy) 스폰 시작
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			FVector SpawnLocation = GetActorLocation();
			switch (SpawnedCubes.Num())
			{
			// Middle Freddy
			case 0: 
				SpawnLocation += FVector(-60, 4580, 530);
				break;
			// Right Freddy
			case 1:
				SpawnLocation += FVector(-530, 4580, 530);
				break;
			// Left Freddy
			case 2:
				SpawnLocation += FVector(420, 4580, 530);
				break;
			}

			FRotator SpawnRotation = FRotator::ZeroRotator;
			AActor* SpawnedCube = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

			if (SpawnedCube)
			{
				UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(SpawnedCube);
				MeshComp->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.SkeletalMesh'/Engine/EngineMeshes/SkeletalCube.SkeletalCube'")).Object);
				MeshComp->SetupAttachment(SpawnedCube->GetRootComponent());
				SpawnedCube->SetRootComponent(MeshComp);
				SpawnedCubes.Add(SpawnedCube);
			}
		}
	}
}

void AEnemyFreddy::ShrinkAndRemoveCubes()
{
	// 큐브 스폰 순서
	if (ShrinkIndex < SpawnedCubes.Num())
	{
		AActor* CubeToShrink = nullptr;
		switch (SpawnedCubes.Num())
		{
			case 3:
				CubeToShrink = (ShrinkIndex == 0) ? SpawnedCubes[2] : (ShrinkIndex == 1) ? SpawnedCubes[1] :SpawnedCubes[0];
				break;
			case 2:
				CubeToShrink = (ShrinkIndex == 0) ? SpawnedCubes[1] : SpawnedCubes[0];
				break;
			case 1:
				CubeToShrink = SpawnedCubes[0];
				break;
		}
		if (CubeToShrink)
		{
			UStaticMeshComponent* MeshComp = CubeToShrink->FindComponentByClass<UStaticMeshComponent>();
			if (MeshComp)
			{
				FVector NewScale = MeshComp->GetComponentScale() * 0.9f;
				MeshComp->SetWorldScale3D(NewScale);

				// 만약 큐브의 Scale이 0.1f 이하라면 Destroy
				if (NewScale.GetMin() < 0.1f)
				{
					SpawnedCubes.Remove(CubeToShrink);
					CubeToShrink->Destroy();
					ShrinkIndex++;
					// 0.5초마다 큐브 remove 시작
					GetWorld()->GetTimerManager().SetTimer(ShrinkTimerHandle, this, &AEnemyFreddy::ShrinkAndRemoveCubes, 0.5f, false);
				}
			}
		}
	}
	else
	{
		// 모든 과정이 끝난 후 Index 초기화
		ShrinkIndex = 0;
		GetWorld()->GetTimerManager().ClearTimer(ShrinkTimerHandle);
	}
}

int32 AEnemyFreddy::GetRandomNumber()
{
	return FMath::RandRange(1,20);
}

bool AEnemyFreddy::IsPlayerLookingAtBedAndFlashOn()
{
	if (Player)
	{
		return Player->GetLookAtState() == AFreddyPlayer::LookAt::Bed && Player->GetFlash();
	}
	return false;
}

void AEnemyFreddy::StartShrinkingCubes()
{
	ShrinkIndex = 0;
	ShrinkAndRemoveCubes();
}

