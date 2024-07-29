// Fill out your copyright notice in the Description page of Project Settings.


#include "JYS/EnemyBonnie.h"

// Sets default values
AEnemyBonnie::AEnemyBonnie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Room0 
	RoomPositions[0] = FVector(0.0f, 0.0f, 0.0f);
	// Room1
	RoomPositions[1] = FVector(100.0f, 100.0f, 100.0f);
	// Room2
	RoomPositions[2] = FVector(200.0f, 200.0f, 200.0f);
	// Room3
	RoomPositions[3] = FVector(300.0f, 300.0f, 300.0f);

	State = EBonnieState::Room3;
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
	int32 Index = static_cast<int32>(MoveState);
	if (Index >= 0 && Index < 4)
	{
		State = MoveState;
		SetActorLocation(RoomPositions[Index]);
	}
}

void AEnemyBonnie::TickRoom0(const float& DeltaTime)
{
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
	int32 RandomNumber = GetRandomNumber();

	if (RandomNumber < Level)
	{
		EBonnieState NewState = State;
		
		switch (State)
		{
			// Room0은 Room1으로만 이동할 수 있다
			case EBonnieState::Room0:
				NewState = EBonnieState::Room1;
				break;
			// Room1은 Room0, Room2으로 이동할 수 있다
			case EBonnieState::Room1:
				NewState = (FMath::RandBool()) ? EBonnieState::Room0 : EBonnieState::Room2;
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

