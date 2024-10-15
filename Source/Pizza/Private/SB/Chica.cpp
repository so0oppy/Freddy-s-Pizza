﻿#include "SB/Chica.h"
#include "SB/LocationState.h"
#include "SB/AILevel.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "HJS/FreddyPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundBase.h"
#include "SB/CupCake.h"
#include "Components/AudioComponent.h"
#include "SB/ChicaAnimInstance.h"
#include "SB/CupCakeAnimInstance.h"

// Sets default values
AChica::AChica()
{
	PrimaryActorTick.bCanEverTick = true;

	AILevelComp = CreateDefaultSubobject<UAILevel>(TEXT("AILevelComp"));

	FootStepsAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootStepsAudioComponent"));
	FootStepsAudioComponent->SetupAttachment(RootComponent);
	FootStepsAudioComponent->bAutoActivate = false; // sound가 바로 재생되지 않게

	BreathAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BreathAudioComponent"));
	BreathAudioComponent->SetupAttachment(RootComponent);
	BreathAudioComponent->bAutoActivate = false; // sound가 바로 재생되지 않게
}

// Called when the game starts or when spawned
void AChica::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Game Start"));

	TagArr.Add(FVector(0,0,0)); //[0]
	TagArr.Add(FindActorsWithTag(FName("Room1"))); //[1]
	TagArr.Add(FindActorsWithTag(FName("Room2"))); //[2]
	TagArr.Add(FindActorsWithTag(FName("Room3"))); //[3]
	TagArr.Add(FindActorsWithTag(FName("Room4"))); //[4]
	TagArr.Add(FindActorsWithTag(FName("Room5"))); //[5]
	TagArr.Add(FindActorsWithTag(FName("Room6"))); //[6]
	TagArr.Add(FindActorsWithTag(FName("Room7"))); //[7]
	TagArr.Add(FindActorsWithTag(FName("Room8"))); //[8]
	TagArr.Add(FindActorsWithTag(FName("Closet"))); //[9]
	TagArr.Add(FindActorsWithTag(FName("Bed"))); //[10]

	UE_LOG(LogTemp, Warning, TEXT("Room array complete"));

	CurrentState = ELocationState::IDLE;

	if ( this->GetMesh() )
	{
		ChicaAnimInstance = Cast<UChicaAnimInstance>(GetMesh()->GetAnimInstance());
	}

	AActor* CupCakeInstance = UGameplayStatics::GetActorOfClass(GetWorld() , ACupCake::StaticClass());
	ACupCake* CupCake = Cast<ACupCake>(CupCakeInstance);
	if ( CupCake->CupcakeComp )
	{
		CupCake->CupcakeAnimInstance = Cast<UCupCakeAnimInstance>(CupCake->CupcakeComp->GetAnimInstance());
	}
}

void AChica::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AChica::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 계속 레벨 확인 (AlLevel 에 있는 SetLevel())
	AILevelComp->SetLevel(this);
	// 계속 State 확인
	UpdateState(DeltaTime);

	// 계속 손전등On/Off, 문 Open/Close 확인
	FlashOn();
	DoorOpen();
}

void AChica::SetUpLocation(ELocationState State, float DeltaTime)
{
	if(CurrentState != State)
	{
		CurrentState = State;

		switch (State)
		{
		case ELocationState::IDLE:	Idle(DeltaTime);
			break;
		case ELocationState::MOVE:	Move();
			break;
		case ELocationState::ATTACK:	Attack();
			break;
		case ELocationState::CUPCAKE:	Cupcake();
			break;
		default:
			break;
		}
	}
}

void AChica::UpdateState(float DeltaTime)
{
	switch (CurrentState)
	{
	case ELocationState::IDLE:	Idle(DeltaTime);
		break;
	case ELocationState::MOVE:	Move();
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

}


void AChica::Idle(float DeltaTime)
{
	//---------------------------------------------------------------------------
	// 만약, 보니가 teleport한 후면 치카는 어디에 있든 움직이지 않음
	AFreddyPlayer* FreddyPlayer = Cast<AFreddyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld() , 0));
	if ( FreddyPlayer->bTeleport == true )
		return;

	// 만약, 손전등 안 켜면 mesh가 안 보이게 설정
	if ( bIsFlashlightOn == false )
		this->GetMesh()->SetVisibility(false);
	else
		this->GetMesh()->SetVisibility(true);

	//---------------------------------------------------------------------------
	AFreddyPlayer::LookAt LookState;
	LookState = FreddyPlayer->GetLookAtState();

	if ( bMoving == true )
	{
		dir = TagArr[1] - GetActorLocation();
		float Distance = dir.Size();

		if ( Distance < 2500.f )
		{
			SetActorLocation(TagArr[1]); // 정확히 목표 위치에 위치시킴
			RoomNum = 1;
			CurrentState = ELocationState::MOVE;
			bMoving = false;
		}
		else
		{
			dir.Normalize();
			SetActorLocation(GetActorLocation() + dir * Speed * DeltaTime); // 위치 업데이트
		}
	}

	// 현재 위치 == room1 || room3 || room4 || room6 || room8 가능
	if ( RoomNum == 6 )
	{
		if ( FreddyPlayer ) // 플레이어와 연동될 부분
		{
			//	만약, 손전등 ON -> room1로 이동 (순간이동X)
			if ( bIsFlashlightOn == true && LookState == AFreddyPlayer::LookAt::Right )
			{
				StopFootStepsSound(); // 발소리 멈춤

				bMoving = true;
				
			}
			// 문 닫으면 -> room8로 이동
			else if ( bIsDoorClose == true && LookState == AFreddyPlayer::LookAt::Right )
			{
				SetActorLocation(TagArr[8]);
				RoomNum = 8;
			}
		}
	}

	else if ( RoomNum == 8 )
	{
		// 발소리는 멈춤
		StopFootStepsSound();

		// 컵케이크 타이머 시작 (혹은 재시작)
		CupCakeTimer += DeltaTime;

		if ( FreddyPlayer ) // 플레이어와 연동될 부분
		{
			LookState = FreddyPlayer->GetLookAtState();

			// →플레이어 위치 == Door : 숨소리들림
			if(LookState == AFreddyPlayer::LookAt::Right )
			{
				// 숨소리 재생
				PlayBreathSound();

				// 문 안 닫고 있으면 멈춤
				if(bIsDoorClose == false )
				{
					//→ 플레이어가 문 안닫고 && 손전등 ON : 점프스케어(공격) 
					if ( bIsFlashlightOn == true )
					{
						if ( FreddyPlayer->KeepJumpScare() == false ) {CurrentState = ELocationState::ATTACK;}
					}
				}
				//→ 문 닫았을 때 
				else if ( bIsDoorClose == true )
				{
					// (중간에서 문 앞으로 텔레포트 시) 안 움직이다가 문을 잠깐 열고 닫으면 그 때 3으로 이동 -> bTeleport == false 일 때 움직임
					// 이동 시, 1.5초 뒤에 돌아가도록 설정
					CurrentTime += DeltaTime;
					if(CurrentTime > 1.5f )
					{
						SetActorLocation(TagArr[3]);
						RoomNum = 3;
					}
				}
			}
			// 문에 안 가면 숨소리 안 들리게
			else if( LookState != AFreddyPlayer::LookAt::Right )
			{
				StopBreathSound();
			}
			
			
			//→ 플레이어 위치= Main 혹은 Bed 일 때, 일정 시간 후에 컵케이크 점프스케어(공격) → GAME OVER
			if ( LookState == AFreddyPlayer::LookAt::Main || LookState == AFreddyPlayer::LookAt::Bed)
			{
				if ( CupCakeTimer > CupCakeTime )
				{
					if ( FreddyPlayer->KeepJumpScare() == false ) {CurrentState = ELocationState::CUPCAKE;}
				}
			}
		}
	}


	// 4.98초마다 AILevel에 있는 RandomMove() 호출 && Move로 상태전이
	CurrentTime += DeltaTime;

	if (CurrentTime > MovableTime) // 이동 가능한 시간이 되면
	{
		// RandomMove가 true일 때만 move
		if(AILevelComp->RandomMove(this, DeltaTime) == true)
		{

			if(RoomNum != 8) // room8이 아닐 때는 Move()
			{
				CurrentState = ELocationState::MOVE;
			}

			else if (RoomNum == 8)  //room8일 때 'attack, cupcake, 이동' 세가지 조건이므로 따로 분류
			{
				
				// → 이동 시간이 됐을 때 플레이어 위치 != Right 이면, 6으로 돌아감 (컵케이크 카운트는 계속)
				if ( LookState != AFreddyPlayer::LookAt::Right )
				{
					SetActorLocation(TagArr[6]);
					RoomNum = 6;
				}
			}
		}
		else
		{
			AILevelComp->RandomMove(this, DeltaTime);
		}
		CurrentTime = 0.f;
	}
}

void AChica::Move() // 손전등 켜고 있으면 1,3,4로만 이동
{
	UE_LOG(LogTemp, Warning, TEXT("Chica Move()"));
	FVector CurrentLocation = this->GetActorLocation();
	// 치카 위치가 room number 몇 인지

	//---------------------------------------------------------------------------
	// 만약, 보니가 teleport한 후면 치카는 어디에 있든 움직이지 않음
	AFreddyPlayer* FreddyPlayer = Cast<AFreddyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld() , 0));
	AFreddyPlayer::LookAt LookState;
	LookState = FreddyPlayer->GetLookAtState();
	if ( FreddyPlayer->bTeleport == true )
		return;
	//---------------------------------------------------------------------------

	// room1 || room4 -> room3 가능
	if (RoomNum == 1 || RoomNum == 4)
	{
		CupCakeTimer = 0.f;
		// 플레이어가 오른쪽 문에 있고 문이 열려있을 때에는 안 움직이도록 설정
		if(LookState != AFreddyPlayer::LookAt::Right || bIsDoorClose == true )
		{
			SetActorLocation(TagArr[3]);
			RoomNum = 3;
		}
	}
	// room3 -> room1 || room4 || room6 가능
	else if (RoomNum == 3)
	{
		CupCakeTimer = 0.f;
		StopBreathSound(); // 숨소리 안 들리게
		PlayFootStepsSound(); //발자국 소리 들리게
		UE_LOG(LogTemp, Log, TEXT("Chica footstep"));

		if ( LookState != AFreddyPlayer::LookAt::Right || bIsDoorClose == true )
		{
			TArray<int32> RoomTags = { 1, 4, 6 };
			int32 RandomIndex = FMath::RandRange(0 , RoomTags.Num() - 1);

			SetActorLocation(TagArr[RoomTags[RandomIndex]]);
			RoomNum = RoomTags[RandomIndex];
		}
	}
	// room6 -> room8 가능
	else if (RoomNum == 6)
	{
		StopBreathSound(); // 숨소리 안 들리게
		PlayFootStepsSound(); //발자국 소리 들리게
		UE_LOG(LogTemp , Log , TEXT("Chica footstep"));

		if ( LookState != AFreddyPlayer::LookAt::Right || bIsDoorClose == true)
		{
			SetActorLocation(TagArr[8]);
			RoomNum = 8;
		}
	}

	CurrentState = ELocationState::IDLE;
}

void AChica::Attack()
{
	// 카메라 앞으로 SetActorLocation
	AFreddyPlayer* FreddyPlayer = Cast<AFreddyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FTransform JmpScare = FreddyPlayer->GetCameraTransform();
	JmpScare.SetLocation(FVector(2157.41f , 2573.72f , 767.85f)); // 위치 조정
	FRotator rot = FRotator(-1.7f , 25.4f , 19.2f);
//	rot.Yaw += 90.0;
	JmpScare.SetRotation(rot.Quaternion());
	SetActorTransform(JmpScare); // 카메라 위치로 이동 (점프스케어)
// 	this->SetActorLocation(FVector(2157.41f , 2573.72f , 767.85f));
// 	this->SetActorRotation(FRotator(-1.7f , 25.4f , 19.2f));

	if ( bJSound == false )
	{
		// 점프스퀘어 anim 재생
		this->GetMesh()->SetVisibility(true);
		ChicaAnimInstance->IsJumpScare = true;
		PlayJumpScare();

		// 점프스케어 소리 재생
		UGameplayStatics::PlaySound2D(this , JumpScareSFX);
		bJSound = true;

		FreddyPlayer->OnDie(TEXT("Chica"));

		UE_LOG(LogTemp , Warning , TEXT("Chica Attack !"));
	}
	// 게임 오버
	
}

void AChica::Cupcake()
{
	// 카메라 앞으로 SetActorLocation
	AFreddyPlayer* FreddyPlayer = Cast<AFreddyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AActor* CupCakeInstance = UGameplayStatics::GetActorOfClass(GetWorld(), ACupCake::StaticClass());
	ACupCake* CupCake = Cast<ACupCake>(CupCakeInstance);
// 	FTransform JmpScare = FreddyPlayer->GetCameraTransform();
// 	JmpScare.SetLocation(JmpScare.GetLocation() - FVector(0 , 300 , 0)); // 위치 조정


	if(CupCake)
	{
// 		CupCake->SetActorTransform(JmpScare); // 카메라 위치로 이동 (점프스케어)
		CupCake->SetActorRelativeLocation(FVector(-50.2f , 3587.85f , 556.48f));
		CupCake->SetActorRelativeRotation(FRotator(10.f , 3.33f , 0.f));

		if(bJSound == false)
		{	
			// 컵케이크 점프스퀘어 anim 재생
			CupCake->CupcakeAnimInstance->IsJumpScare = true;
			PlayCupCakeJumpScare();

			// 점프스케어 소리 재생
			UGameplayStatics::PlaySound2D(this , CupCakeScareSFX);
			bJSound = true;

			FreddyPlayer->OnDie(TEXT("CupCake"));

			UE_LOG(LogTemp , Warning , TEXT("CupCake Attack !"));
		}
	}

	// 게임 오버
}

void AChica::PlayJumpScare()
{
	if ( ChicaAnimInstance )
	{
		ChicaAnimInstance->PlayJumpScareMontage();
	}
}

void AChica::PlayCupCakeJumpScare()
{
	AActor* CupCakeInstance = UGameplayStatics::GetActorOfClass(GetWorld() , ACupCake::StaticClass());
	ACupCake* CupCake = Cast<ACupCake>(CupCakeInstance);

	if ( CupCake->CupcakeAnimInstance )
	{
		CupCake->CupcakeAnimInstance->PlayJumpScareMontage();
	}
}


FVector AChica::FindActorsWithTag(FName Tag)
{
	// 각 방의 위치 정보를 태그로 받아와서 배열로 저장
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);

	if (FoundActors.Num() > 0)
	{
		AActor* TargetActor = FoundActors[0];
		FVector TargetLocation = TargetActor->GetActorLocation();

		return TargetLocation;
		// 배열 인덱스 값 = 방 번호
	}

	return FVector::ZeroVector; // FoundActors가 비어있을 경우, 기본값 반환
}


void AChica::CanMove()
{
	RoomNum = 1;

	SetActorLocation(TagArr[1]);
	CurrentState = ELocationState::IDLE;
}

// 손전등 상태 가져오는 함수
void AChica::FlashOn()
{
	AFreddyPlayer* FreddyPlayer = Cast<AFreddyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(FreddyPlayer)
	{
		bIsFlashlightOn = FreddyPlayer->GetFlash();
	}
}

// 문 상태 가져오는 함수
void AChica::DoorOpen()
{
	AFreddyPlayer* FreddyPlayer = Cast<AFreddyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if(FreddyPlayer)
	{
		bIsDoorClose = FreddyPlayer->GetrCloseDoor();
	}
}

void AChica::PlayFootStepsSound()
{
	if ( FootStepsSFX && !FootStepsAudioComponent->IsPlaying() )
	{
		FootStepsAudioComponent->SetSound(FootStepsSFX);
		FootStepsAudioComponent->Play();
	}
}

void AChica::StopFootStepsSound()
{
	if ( FootStepsAudioComponent->IsPlaying() )
	{
		FootStepsAudioComponent->Stop();
	}
}

void AChica::PlayBreathSound()
{
	if ( BreathSFX && !BreathAudioComponent->IsPlaying() )
	{
		BreathAudioComponent->SetSound(BreathSFX);
		BreathAudioComponent->Play();
	}
}

void AChica::StopBreathSound()
{
	if ( BreathAudioComponent->IsPlaying() )
	{
		BreathAudioComponent->Stop();
	}
}