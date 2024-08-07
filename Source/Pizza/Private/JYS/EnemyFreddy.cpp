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
	
	SetAILevel(20);

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

	if ( AddFreddyFastly() )
	{
		// 시간이 흐르면서
		CurrentTime += DeltaTime;
		if ( CurrentTime >= 15 )
		{
			SpawnFreddyTimerOnce = false;
			CurrentTime = 0;
			GetWorld()->GetTimerManager().SetTimer(FreddysVisibleFastlyTimerHandle , this , &AEnemyFreddy::AttemptSpawnCube , 1.5f , true);
		}
	}
	else if (NotLookingAtTheDoor())
	{
		if ( SpawnFreddyTimerOnce == false )
		{
			SpawnFreddyTimerOnce = true;
			GetWorld()->GetTimerManager().SetTimer(FreddysVisibleTimerHandle , this , &AEnemyFreddy::AttemptSpawnCube , 3.02f , true);
		}

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
			// 프레디가 3마리이고 3초 이상이 지나면 점프스케어
			if (FreddyMesh2->bHiddenInGame == false && HiddenTime >= 3)
			{
				if ( IsPlayerLookingAtBed() )
				{
					JumpScareFreddy();
					JumpScareFreddySound();
				}
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

bool AEnemyFreddy::AddFreddyFastly()
{
	if ( Player )
	{
		return Player->GetLookAtState() == AFreddyPlayer::LookAt::Main;
	}
	return false;
}

bool AEnemyFreddy::NotLookingAtTheDoor()
{
	if ( Player )
	{
		return Player->GetLookAtState() == AFreddyPlayer::LookAt::Bed || Player->GetLookAtState() == AFreddyPlayer::LookAt::Left || Player->GetLookAtState() == AFreddyPlayer::LookAt::Center || Player->GetLookAtState() == AFreddyPlayer::LookAt::Right || Player->GetLookAtState() == AFreddyPlayer::LookAt::Back;
	}
	return false;
}

bool AEnemyFreddy::IsPlayerLookingAtBed()
{
	// GetMesh()->SetHiddenInGame(false);
	if ( Player )
	{
		return Player->GetLookAtState() == AFreddyPlayer::LookAt::Bed;
	}
	return false;
}

void AEnemyFreddy::JumpScareFreddy()
{
	if ( IsPlayerLookingAtBed() == true )
	{
		FVector CameraLoc = Player->GetCameraTransform().GetLocation();
		FVector CameraLocForwardVector = CameraLoc.RightVector * 400;
		CameraLoc += CameraLocForwardVector;
		SetActorLocation(CameraLoc);
	}
	else
	{
		FVector CameraLoc = Player->GetCameraTransform().GetLocation();
		CameraLoc.Y -= 100;
		CameraLoc.Z -= 60;
		SetActorLocation(CameraLoc);
	}

	GetMesh()->SetHiddenInGame(false);

	Player->OnDie();

	for (int32 i = 0; i < FreddysArr.Num(); ++i)
	{
		FreddysArr[i]->SetHiddenInGame(true);
	}
}

