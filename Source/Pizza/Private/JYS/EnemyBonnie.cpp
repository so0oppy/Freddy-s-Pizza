// Fill out your copyright notice in the Description page of Project Settings.


#include "JYS/EnemyBonnie.h"
#include "HJS/FreddyPlayer.h"
#include "EngineUtils.h"

// Sets default values
AEnemyBonnie::AEnemyBonnie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Room0 
	RoomPositions[0] = FVector(-320.0f, 810.0f, 0.0f);
	// Room1
	RoomPositions[1] = FVector(-320.0f, 360.0f, 0.0f);
	// Room2
	RoomPositions[2] = FVector(-320.0f, -220.0f, 0.0f);
	// Room3
	RoomPositions[3] = FVector(200.0f, -280.0f, 0.0f);

	State = EBonnieState::Room1;

	Player = nullptr; // Initialize Player to nullptr
	TargetLocation = RoomPositions[0]; // Initialize TargetLocation
	// 이동 속도 초기화
	MoveSpeed = 500.0f;
	// 이동 중 여부 초기화
	bIsMovingToRoom3 = false;
}

// Called when the game starts or when spawned
void AEnemyBonnie::BeginPlay()
{
	Super::BeginPlay();
	
	SetAILevel(7);

	GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &AEnemyBonnie::AttemptMove, 4.97f, true);
}

// Called every frame
void AEnemyBonnie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Room1에서 Room3로 이동하기
	if (ShouldMoveToRoom3())
	{
		Move(EBonnieState::Room3);
	}
	
	// Room0에서 Room2로 이동
	if (CloseDoorRoom0ToRoom2())
	{
		Move(EBonnieState::Room2);
	}

	if (bIsMovingToRoom3)
	{
		// 현재 위치를 목표 위치로 보간하여 이동
		FVector Dir = TargetLocation - GetActorLocation();
		Dir.Normalize();
		FVector P0 = GetActorLocation();
		SetActorLocation(P0 + Dir * DeltaTime * MoveSpeed);

		// 목표 위치에 도달하면 이동 중 상태 해제
		if (FVector::Dist(GetActorLocation(), TargetLocation) < 1.0f)
		{
			bIsMovingToRoom3 = false;
		}
	}

	switch (State)
	{
		case EBonnieState::Room0:	
			TickRoom0(DeltaTime);	
			break;
		case EBonnieState::Room1:	
			TickRoom1(DeltaTime);	
			break;
		case EBonnieState::Room2:	
			TickRoom2(DeltaTime);	
			break;
		case EBonnieState::Room3:	
			TickRoom3(DeltaTime);	
			break;
	}
}

void AEnemyBonnie::Move(EBonnieState MoveState)
{
	if (State == EBonnieState::Room1 && MoveState == EBonnieState::Room3)
	{
		UE_LOG(LogTemp,Warning,TEXT("1234"));
		// 1번방에서 3번방으로 이동할 때 이동	
		bIsMovingToRoom3 = true;
		TargetLocation = RoomPositions[static_cast<int32>(MoveState)];
	}
	else
	{
		// 다른 방으로 이동할 때는 순간이동
		bIsMovingToRoom3 = false;
		int32 Index = static_cast<int32>(MoveState);
		if (Index >= 0 && Index < 4)
		{
			State = MoveState;
			SetActorLocation(RoomPositions[Index]);
		}
	}
}

void AEnemyBonnie::TickRoom0(const float& DeltaTime)
{
	if (ShouldMoveToRoom3())
	{
		JumpScareBonnie();
	}
}

void AEnemyBonnie::TickRoom1(const float& DeltaTime)
{
}

void AEnemyBonnie::TickRoom2(const float& DeltaTime)
{
}

void AEnemyBonnie::TickRoom3(const float& DeltaTime)
{
}

void AEnemyBonnie::AttemptMove()
{
	// 랜덤 1~20 숫자 뽑기
	int32 RandomNumber = GetRandomNumber();

	// 랜덤 넘버가 보니의 레벨보다 낮을 때 이동
	if (RandomNumber < Level)
	{
		EBonnieState NewState = State;
		
		switch (State)
		{
			// Room0은 Room1으로만 이동할 수 있다
			case EBonnieState::Room0:
				NewState = EBonnieState::Room1;
				break;
			// Room1은 Room0, Room2, Room3로 이동할 수 있다
			case EBonnieState::Room1:
			{
				int32 MoveChoice = FMath::RandRange(0, 2);
				switch (MoveChoice)
				{
				case 0:
					NewState = EBonnieState::Room0;
					break;
				case 1:
					NewState = EBonnieState::Room2;
					break;
				case 2:
					NewState = EBonnieState::Room3;
					break;
				}
			}	
				break;
			// Room2는 Room1이랑 Room3으로 이동할 수 있다
			case EBonnieState::Room2:
				NewState = (FMath::RandBool()) ? EBonnieState::Room1 : EBonnieState::Room3;
				break;
			// Room3은 Room2로만 이동할 수 있다
			case EBonnieState::Room3:
				NewState = EBonnieState::Room2;
				break;
		}
		Move(NewState);
	}
}

int32 AEnemyBonnie::GetRandomNumber()
{
	return FMath::RandRange(1, 20);
}

bool AEnemyBonnie::ShouldMoveToRoom3()
{
	// 플레이어가 왼쪽에서 Flash를 비추고 Bonnie가 1번방에 있을 때
	if (Player)
	{
		return Player->GetFlash() && Player ->GetLookAtState() == AFreddyPlayer::LookAt::Left && State == EBonnieState::Room1;
	}
	return false;      
}

void AEnemyBonnie::JumpScareBonnie()
{
	FVector CameraLoc = Player->GetCameraTransform().GetLocation();
	CameraLoc.X += 100;
	SetActorLocation(CameraLoc);
}

bool AEnemyBonnie::CloseDoorRoom0ToRoom2()
{
	if (Player)
	{
		return Player->GetrCloseDoor() && Player->GetLookAtState() == AFreddyPlayer::LookAt::Left && State == EBonnieState::Room0;
	}
	return false;
}
