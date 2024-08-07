// Fill out your copyright notice in the Description page of Project Settings.


#include "JYS/EnemyFreddy.h"
#include "HJS/FreddyPlayer.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyFreddy::AEnemyFreddy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Freddy 3마리 만들기
	Freddy0 = CreateDefaultSubobject<UBoxComponent>(TEXT("Freddy0"));
	Freddy0->SetRelativeLocation(FVector(- 60.0f, 4580.0f, 530.0f));
	FreddyMesh0 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FreddyMesh0"));
	FreddyMesh0->SetupAttachment(Freddy0);
	FreddyMesh0->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Freddy1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Freddy1"));
	Freddy1->SetRelativeLocation(FVector(-530.0f, 4580.0f, 530.0f));
	FreddyMesh1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FreddyMesh1"));
	FreddyMesh1->SetupAttachment(Freddy1);
	FreddyMesh1->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Freddy2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Freddy2"));
	Freddy2->SetRelativeLocation(FVector(4200.0f, 4580.0f, 530.0f));
	FreddyMesh2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FreddyMesh2"));
	FreddyMesh2->SetupAttachment(Freddy2);
	FreddyMesh2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// JumpScare Sound
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/SFX/FNAFSFX01/Scream3.Scream3'"));
	if (tempSound.Succeeded()) 
	{
		JumpScareFreddySFX = tempSound.Object;
	}

}

// Called when the game starts or when spawned
void AEnemyFreddy::BeginPlay()
{
	Super::BeginPlay();
	
	SetAILevel(3);

	// 큐브(Freddy) 스폰 타이머
	GetWorld()->GetTimerManager().SetTimer(FreddysVisibleTimerHandle, this, &AEnemyFreddy::AttemptSpawnCube, 3.02f, true);

	// Freddy들을 배열에 넣어 줌
	FreddysArr.Add(FreddyMesh0);
	FreddysArr.Add(FreddyMesh1);
	FreddysArr.Add(FreddyMesh2);

	// Freddy들을 처음에 Hide 해줌
	for (int32 i = 0; i < FreddysArr.Num(); ++i)
	{
		FreddysArr[i]->SetHiddenInGame(true);
	}

	GetMesh()->SetHiddenInGame(true);
}

// Called every frame
void AEnemyFreddy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPlayerLookingAtBedAndFlashOn())
	{
		HideFreddy(DeltaTime);
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
			if (FreddyMesh2->bHiddenInGame == false && HiddenTime >= 3)
			{
				JumpScareFreddy();
				JumpScareFreddySound();
			}
			// Freddy를 차례대로 Visible
			if (FreddyMesh0->bHiddenInGame)
			{
				HiddenTime += 1;
				FreddyMesh0->SetHiddenInGame(false);
			}
			else if (!FreddyMesh0->bHiddenInGame)
			{
				if (FreddyMesh1->bHiddenInGame)
				{
					HiddenTime += 1;
					FreddyMesh1->SetHiddenInGame(false);
				}
				else
				{
					HiddenTime += 1;
					FreddyMesh2->SetHiddenInGame(false);
				}
			}
		}
	}
}

void AEnemyFreddy::HideFreddy(float DeltaTime)
{
	HiddenTime -= DeltaTime;
	if (HiddenTime > 1 && HiddenTime < 2)
	{
		FreddyMesh2->SetHiddenInGame(true);
	}
	else if (HiddenTime > 0 && HiddenTime < 1)
	{
		FreddyMesh1->SetHiddenInGame(true);
	}
	else if (HiddenTime <= 0)
	{
		FreddyMesh0->SetHiddenInGame(true);
		HiddenTime = 0;
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

void AEnemyFreddy::JumpScareFreddySound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), JumpScareFreddySFX);
}

void AEnemyFreddy::JumpScareFreddy()
{
	FVector CameraLoc = Player->GetCameraTransform().GetLocation();
	CameraLoc.Y -= 100;
	CameraLoc.Z -= 60;
	SetActorLocation(CameraLoc);

	GetMesh()->SetHiddenInGame(false);

	Player->OnDie();

	for (int32 i = 0; i < FreddysArr.Num(); ++i)
	{
		FreddysArr[i]->SetHiddenInGame(true);
	}
}

