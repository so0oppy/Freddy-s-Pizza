#include "Chica.h"
#include "LocationState.h"
#include "../Public/HJS/FreddyPlayer.h"
#include "AILevel.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AChica::AChica()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = ELocationState::IDLE;

}

// Called when the game starts or when spawned
void AChica::BeginPlay()
{
	Super::BeginPlay();
	
	// UAILevel 컴포넌트 가져오기
	AILevelComp = FindComponentByClass<UAILevel>();
}

// Called every frame
void AChica::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 계속 레벨 확인 (AlLevel 에 있는 SetLevel())
	AILevelComp->SetLevel(this);
}

void AChica::SetUpLocation(ELocationState State, float DeltaTime, FName Tag)
{
	switch (State)
	{
	case ELocationState::IDLE:	Idle(DeltaTime, Tag);
		break;
	case ELocationState::MOVE:	Move(Tag);
		break;
	case ELocationState::ATTACK:	Attack();
		break;
	case ELocationState::CUPCAKE:	Cupcake();
		break;
	default:
		break;
	}
}

// Called to bind functionality to input
void AChica::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 컨트롤 키 입력 설정
	PlayerInputComponent->BindAction("MoveToRoom1", IE_Pressed, this, &AChica::OnCtrlKeyPressed);

}

FName AChica::GetCurrentLocationTag()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->ActorHasTag("Room1"))
		{
			return FName("Room1");
		}
		else if (Actor->ActorHasTag("Room3"))
		{
			return FName("Room3");
		}
		else if (Actor->ActorHasTag("Room4"))
		{
			return FName("Room4");
		}
		else if (Actor->ActorHasTag("Room6"))
		{
			return FName("Room6");
		}
		else if (Actor->ActorHasTag("Room8"))
		{
			return FName("Room8");
		}
	}

	return FName("");
}

void AChica::Idle(float DeltaTime, FName Tag)
{
	// 현재 위치 == room1 || room3 || room4 || room6 || room8 가능
	
	// 4.98초마다 AILevel에 있는 RandomMove() 호출 && Move로 상태전이
	CurrentTime += DeltaTime;
	if (CurrentTime > MovableTime)
	{
		AILevelComp->RandomMove(this, DeltaTime, Tag); // RandomMove안에 상태전이 있음

		CurrentTime = 0.f;
	}
}

void AChica::Move(FName Tag) // 손전등 켜고 있으면 1,3,4로만 이동
{
	FName CurrentTag = GetCurrentLocationTag();

	// room1 || room4 -> room3 가능
	if (CurrentTag == "Room1" || CurrentTag == "Room4")
	{
		MoveToTaggedLocation(FName("Room3"));
	}
	// room3 -> room1 || room4 || room6 가능
	else if (CurrentTag == "Room3")
	{
		TArray<FName> RoomTags = { FName("Room1"), FName("Room4"), FName("Room6") };
		int32 RandomIndex = FMath::RandRange(0, RoomTags.Num() - 1);

		MoveToTaggedLocation(RoomTags[RandomIndex]);
	}
	// room6 -> room3 || room8 가능
	else if (CurrentTag == "Room6")
	{
		TArray<FName> RoomTags = { FName("Room3"), FName("Room8") };
		int32 RandomIndex = FMath::RandRange(0, RoomTags.Num() - 1);

		MoveToTaggedLocation(RoomTags[RandomIndex]);

		// 발소리

		//	만약, 손전등 ON -> room1로 이동 (순간이동X)
		if(bIsFlashlightOn) 
		{
			// 손전등 ON && Door: Close -> room8로 이동
			if (!bIsDoorOpen)
			{
				MoveToTaggedLocation(FName("Room8"));
			}
			MoveToTaggedLocation(FName("Room1"));
		}
	}
	// room8 -> room6 가능
	else if (CurrentTag == "Room8")  // <<<<<(추후 Door에 있을 때 위치정보 받아와야 함)
	{
		Cast<AFreddyPlayer>
		//→ 플레이어 위치 == Door && 손전등 ON : 점프스퀘어(공격) 
		if (bIsDoorOpen && bIsFlashlightOn)
		{
			CurrentState = ELocationState::ATTACK;
		}
		//→ 플레이어 위치 == Door && bCLOSE == true (일정 시간동안 CLOSE ⇒ 확률적으로 1,3,4 중 이동)
		if (!bIsDoorOpen)
		{	
			for (float cnt = 0.f; cnt < 6.f; cnt++)
			{
				if (cnt > MovableTime)
				{
					TArray<FName> RoomTags = { FName("Room1"), FName("Room3"), FName("Room4") };
					int32 RandomIndex = FMath::RandRange(0, RoomTags.Num() - 1);

					MoveToTaggedLocation(RoomTags[RandomIndex]);
				}
			}
		}
		
		//→ 플레이어 위치≠Door 일 때, 일정 시간 후에 컵케이크 점프스퀘어(공격) → GAME OVER
		if (bIsDoorOpen)
		{
			CurrentState = ELocationState::CUPCAKE;
		}
		
		MoveToTaggedLocation(FName("Room6"));
	}
}

void AChica::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack !"));
	// 점프스퀘어 anim 재생

	// 게임 오버
	
}

void AChica::Cupcake()
{
	UE_LOG(LogTemp, Warning, TEXT("CupCake Attack !"));
	// 컵케이크 점프스퀘어 anim 재생

	// 게임 오버
}

void AChica::MoveToTaggedLocation(FName Tag)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);

	if (FoundActors.Num() > 0)
	{
		AActor* TargetActor = FoundActors[0];
		FVector TargetLocation = TargetActor->GetActorLocation();

		UE_LOG(LogTemp, Warning, TEXT("Moving to Location: %s"), *Tag.ToString());

		GetController()->StopMovement();

		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalLocation(TargetLocation);
			MoveRequest.SetAcceptanceRadius(5.0f); // 목표 위치에 도달하는 범위 설정

			FNavPathSharedPtr NavPath;
			AIController->MoveTo(MoveRequest, &NavPath);
		}
	}
}

//void AChica::EndPlay(const EEndPlayReason::Type EndPlayReason)
//{
//	Super::EndPlay(EndPlayReason);
//
//	GetWorld()->GetTimerManager().ClearTimer(Timer);
//}

// 손전등 ON 함수
void AChica::OnCtrlKeyPressed()
{
	bIsFlashlightOn = !bIsFlashlightOn; // 손전등 상태 토글
}

