// Fill out your copyright notice in the Description page of Project Settings.


#include "JYS/EnemyBonnie.h"
#include "HJS/FreddyPlayer.h"
#include "EngineUtils.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "JYS/BonnieAnimInstance.h"

// Sets default values
AEnemyBonnie::AEnemyBonnie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Room0 
	RoomPositions[0] = FVector(-320.0f, 810.0f, 0.0f);
	// Room1
	RoomPositions[1] = FVector(-2018.149391f, -1358.837438f, 0.0f);
	// Room2
	RoomPositions[2] = FVector(-320.0f, -220.0f, 0.0f);
	// Room3
	RoomPositions[3] = FVector(200.0f, -280.0f, 0.0f);

	State = EBonnieState::Room3;

	Player = nullptr; // Initialize Player to nullptr
	TargetLocation = RoomPositions[0]; // Initialize TargetLocation
	// 이동 속도 초기화
	MoveSpeed = 1000.0f;
	// 이동 중 여부 초기화
	bIsMovingToRoom3 = false;
	
	// JumpScare Sound
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/SFX/FNAFSFX01/Scream2.Scream2'"));
		if (tempSound.Succeeded()) 
		{
			JumpScareSFX = tempSound.Object;
		}
	
	// 발자국 소리
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound1(TEXT("/Script/Engine.SoundWave'/Game/SFX/FNAFSFX02/deepfootsteps1.deepfootsteps1'"));
	if (tempSound.Succeeded()) 
	{
		FootStepsSFX = tempSound1.Object;
	}	
	
	// 숨소리
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound2(TEXT("/Script/Engine.SoundWave'/Game/SFX/FNAFSFX02/animatronicbreath.animatronicbreath'"));
	if (tempSound.Succeeded())
	{
		BreathSFX = tempSound2.Object;
	}

	bIsBreathSoundPlaying = false;
}

// Called when the game starts or when spawned
void AEnemyBonnie::BeginPlay()
{
	Super::BeginPlay();
	
	SetAILevel(20);

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
	if (CloseDoorRoom0ToRoom2() && !Player->bTeleport)
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
	if ( MoveState == EBonnieState::Room1 )
	{
		FootStepsSound();
	}
	bIsMovingToRoom3 = false;
	if (State == EBonnieState::Room1 && MoveState == EBonnieState::Room3)
	{
		UE_LOG(LogTemp,Warning,TEXT("1234"));
		// 1번방에서 3번방으로 이동할 때 이동	
		bIsMovingToRoom3 = true;
		TargetLocation = RoomPositions[static_cast<int32>(MoveState)];
		State = EBonnieState::Room3;
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
	if ( State == EBonnieState::Room0 )
	{
		JumpscareCount += DeltaTime;
	}
	else if ( JumpScareConditions() || CloseDoorRoom0ToRoom2())
	{
		JumpscareCount = 0;
	}
	else if ( State == EBonnieState::Room2 || State == EBonnieState::Room3 )
	{
		JumpscareCount = 0;
	}
	
	if ( JumpscareCount >= 8 && LookingMain() )
	{

		if ( bJumpScare == false )
		{
			bJumpScare = true;
			JumpScareBonnie();
			JumpScareSound();
		}
	}
	
	if ( JumpScareConditions() )
	{

		if ( bJumpScare == false )
		{
			bJumpScare = true;

			JumpScareBonnie();
			JumpScareSound();
		}
	}

	if (BreathSoundConditions())
	{
		BreathSound();
	}

	// 보니가 1번방에서 0번방으로 텔레포트했을 경우 플레이어가 문을 닫으면 이동을 하지 않음 (진성오빠 텔레포트 bool값 가져오기)
	// 보니가 원래 Room0에 있었고 플레이어가 문을 닫으면 1~2초 뒤에 2번방으로 이동
}

void AEnemyBonnie::TickRoom1(const float& DeltaTime)
{
	if ( CloseDoorRoom1ToRoom0() )
	{
		Move(EBonnieState::Room0); 
	}
}

void AEnemyBonnie::TickRoom2(const float& DeltaTime)
{
}

void AEnemyBonnie::TickRoom3(const float& DeltaTime)
{
}

void AEnemyBonnie::AttemptMove()
{

	if (Player->bTeleport)
	{
		return;
	}

	if ( Player->GetLookAtState() == AFreddyPlayer::LookAt::Left )
	{	
		return;
	}
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
			// Room1은 Room0으로 이동할 수 있다
			case EBonnieState::Room1:
			{
				switch ( State )
				{
				case EBonnieState::Room1:
					NewState = EBonnieState::Room0;
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

// 플레이어가 왼쪽에서 Flash를 비추고 Bonnie가 1번방에 있을 때
bool AEnemyBonnie::ShouldMoveToRoom3()
{
	if (Player)
	{
		return Player->GetFlash() && Player ->GetLookAtState() == AFreddyPlayer::LookAt::Left && State == EBonnieState::Room1;
	}
	return false;      
}

void AEnemyBonnie::JumpScareBonnie()
{


	auto* BonnieAnim = Cast<UBonnieAnimInstance>(GetMesh()->GetAnimInstance());
	if ( BonnieAnim ) {
		BonnieAnim->BonnieJumpscareAnimation();
	}

	if ( LookingMain() == true )
	{
		Player->OnDie(TEXT("BonnieMain"));
		FVector CameraLoc = FVector(541.42f , 3355.61f , 1321.16f);
		FRotator CameraRotator = FRotator(0.f , -10.4f , 15.7f);
		SetActorLocation(CameraLoc);
		GetMesh()->SetRelativeRotation(CameraRotator);
	}
	else 
	{
		Player->OnDie(TEXT("BonnieDoor"));
		FVector CameraLoc = FVector(-1695.81f , 2342.84f , 1392.26f);
		FRotator CameraRotator = FRotator(4.23f , -26.95f , 11.87f);
		SetActorLocation(CameraLoc);
		GetMesh()->SetRelativeRotation(CameraRotator);
	}
}

// Bonnie가 0번방에 있고 플레이어가 문을 닫으면 2번방으로 이동
bool AEnemyBonnie::CloseDoorRoom0ToRoom2()
{
	if (Player)
	{
		return Player->GetrCloseDoor() && Player->GetLookAtState() == AFreddyPlayer::LookAt::Left && State == EBonnieState::Room0;
	}
	return false;
}
// 점프스케어 사운드
void AEnemyBonnie::JumpScareSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), JumpScareSFX);
}
// 발소리
void AEnemyBonnie::FootStepsSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), FootStepsSFX);
}
// 숨소리
void AEnemyBonnie::BreathSound()
{

	if (!bIsBreathSoundPlaying)
	{
		bIsBreathSoundPlaying = true;

		UGameplayStatics::PlaySound2D(GetWorld(), BreathSFX);

		// 사운드 길이를 얻어서 그 시간 후에 OnBreathSoundFinished 함수 호출
		float Duration = BreathSFX->GetDuration();
		GetWorld()->GetTimerManager().SetTimer(BreathTimerHandle, this, &AEnemyBonnie::OnBreathSoundFinished, Duration, false);

	}
}
// 숨소리 한번만 재생
void AEnemyBonnie::OnBreathSoundFinished()
{
	bIsBreathSoundPlaying = false;
}
// 문 닫으면 Room1에서 Room0으로 이동
bool AEnemyBonnie::CloseDoorRoom1ToRoom0()
{
	if ( Player )
	{
		return Player->GetrCloseDoor() && Player->GetLookAtState() == AFreddyPlayer::LookAt::Left && State == EBonnieState::Room1;
	}
	return false;

}

bool AEnemyBonnie::LookingMain()
{
// 플레이어가 Main을 보고 있을 때
	if ( Player )
	{
		return Player->GetLookAtState() == AFreddyPlayer::LookAt::Main || Player->GetLookAtState() == AFreddyPlayer::LookAt::Bed;
	}
	return false;
}

bool AEnemyBonnie::BreathSoundConditions()
{
	// 플레이어가 왼쪽에 있고 Bonnie가 0번방에 있을 때 (Breath Sound 재생)
	if (Player)
	{
		return Player->GetLookAtState() == AFreddyPlayer::LookAt::Left;
	}
	return false;
}


bool AEnemyBonnie::JumpScareConditions()
{
	// 플레이어가 왼쪽에서 Flash를 비추기
	if (Player)
	{
		return Player->GetFlash() && Player->GetLookAtState() == AFreddyPlayer::LookAt::Left;
	}
	return false;
}
