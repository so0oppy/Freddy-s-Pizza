// Fill out your copyright notice in the Description page of Project Settings.


#include "SB/Foxy.h"
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
#include "SB/FoxDoll.h"
#include "Components/AudioComponent.h"
#include "SB/FoxyAnimInstance.h"
#include "Engine/StaticMesh.h"

// Sets default values
AFoxy::AFoxy()
{
	PrimaryActorTick.bCanEverTick = true;

	AILevelComp = CreateDefaultSubobject<UAILevel>(TEXT("AILevelComp"));

	FootStepsAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootStepsAudioComponent"));
	FootStepsAudioComponent->SetupAttachment(RootComponent);
	FootStepsAudioComponent->bAutoActivate = false; // sound가 바로 재생되지 않게

	FoxyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoxyMeshComponent"));
	FoxyMeshComponent->SetupAttachment(RootComponent);
	
	// 폭시 상태 Mesh
	ConstructorHelpers::FObjectFinder<UStaticMesh> StateThreeMesh(TEXT("/Script/Engine.StaticMesh'/Game/sym/animation_model/foxy/Mesh/SM_FoxyState3.SM_FoxyState3'"));

	if ( StateThreeMesh.Succeeded() )
	{
		MeshState3 = StateThreeMesh.Object;
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> StateTwoMesh(TEXT("/Script/Engine.StaticMesh'/Game/sym/animation_model/foxy/Mesh/SM_FoxyState2.SM_FoxyState2'"));
	if ( StateTwoMesh.Succeeded() )
	{
		MeshState2 = StateTwoMesh.Object;
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> StateOneMesh(TEXT("/Script/Engine.StaticMesh'/Game/sym/animation_model/foxy/Mesh/SM_FoxyState1.SM_FoxyState1'"));
	if ( StateOneMesh.Succeeded() )
	{
		MeshState1 = StateOneMesh.Object;
	}
}

// Called when the game starts or when spawned
void AFoxy::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Game Start"));

	TagArr.Add(FVector(0, 0, 0)); //[0]
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

	AActor* FoxDollInstance = UGameplayStatics::GetActorOfClass(GetWorld() , AFoxDoll::StaticClass());
	ShowFoxyDoll(FoxDollInstance, false);
	this->GetMesh()->SetVisibility(true); // skeletal 보이게
	ShowFoxy(MeshState3 , false);

	CurrentState = ELocationState::IDLE;

	if ( this->GetMesh() )
	{
		FoxyAnimInstance = Cast<UFoxyAnimInstance>(GetMesh()->GetAnimInstance());
		FakeAnimInstance = Cast<UFoxyAnimInstance>(GetMesh()->GetAnimInstance());
	}
}

void AFoxy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AFoxy::Tick(float DeltaTime)
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
//--------------------------------------------------------------------------------------

void AFoxy::SetUpLocation(ELocationState State, float DeltaTime)
{
	if (CurrentState != State)
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
		case ELocationState::CLOSET:	Closet(DeltaTime);
			break;

		default:
			break;
		}
	}
}

void AFoxy::UpdateState(float DeltaTime)
{
	switch (CurrentState)
	{
	case ELocationState::IDLE:	Idle(DeltaTime);
		break;
	case ELocationState::MOVE:	Move();
		break;
	case ELocationState::ATTACK:	Attack();
		break;
	case ELocationState::CLOSET:	Closet(DeltaTime);
		break;

	default:
		break;
	}
}
//--------------------------------------------------------------------------------------
// Called to bind functionality to input
void AFoxy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
//--------------------------------------------------------------------------------------

void AFoxy::Idle(float DeltaTime)
{
	AFreddyPlayer* FreddyPlayer = Cast<AFreddyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld() , 0));
	AFreddyPlayer::LookAt LookState;
	LookState = FreddyPlayer->GetLookAtState();

	//////////////////////////////////////////////////////////////////////////////
	// 플레이어 위치 == 침대 만 계속 바라보고 있으면 카운트 증가 후 점프스케어
	if ( LookState == AFreddyPlayer::LookAt::Bed )
	{
		ScareCount += DeltaTime;
		if ( ScareCount > 15.f )
		{
			if(FreddyPlayer->KeepJumpScare() == false){CurrentState = ELocationState::ATTACK;}
		}
	}
	// 플레이어 위치가 침대가 아닐 때 스케어카운트 0으로 초기화
	else
		ScareCount = 0.f;
	//////////////////////////////////////////////////////////////////////////////

	if ( RoomNum == 5 )
	{
		if ( FreddyPlayer ) // 플레이어와 연동될 부분
		{
			LookState = FreddyPlayer->GetLookAtState();

			// 플레이어 위치 == 오른쪽Door이면, 바로 9로 이동
			if ( LookState == AFreddyPlayer::LookAt::Right )
			{
				SetActorLocation(TagArr[9]);
				RoomNum = 9;

				this->GetMesh()->SetVisibility(false);
			}

			PlayFootStepsSound();
			this->GetMesh()->SetVisibility(true);

			//	만약, 손전등 ON -> room1로 이동 (순간이동X)
			if ( bIsFlashlightOn == true )
			{
				StopFootStepsSound();

				dir = TagArr[1] - GetActorLocation();
				float Distance = dir.Size();

				if ( Distance < 2000.f )
				{
					SetActorLocation(TagArr[1]); // 정확히 목표 위치에 위치시킴
					RoomNum = 1;
					CurrentState = ELocationState::MOVE;
				}
				else
				{
					dir.Normalize();
					SetActorLocation(GetActorLocation() + dir * Speed * DeltaTime); // 위치 업데이트
				}
			}
		}
	}
	
	else if ( RoomNum == 6 )
	{
		if ( FreddyPlayer ) // 플레이어와 연동될 부분
		{
			LookState = FreddyPlayer->GetLookAtState();

			// 플레이어 위치 == 왼쪽Door이면, 바로 9로 이동
			if ( LookState == AFreddyPlayer::LookAt::Left )
			{
				SetActorLocation(TagArr[9]);
				RoomNum = 9;

				this->GetMesh()->SetVisibility(false);
			}

			PlayFootStepsSound();
			this->GetMesh()->SetVisibility(true);

			//	만약, 손전등 ON -> room1로 이동 (순간이동X)
			if ( bIsFlashlightOn == true )
			{
				StopFootStepsSound();

				dir = TagArr[1] - GetActorLocation();
				float Distance = dir.Size();

				if ( Distance < 2000.f )
				{
					SetActorLocation(TagArr[1]); // 정확히 목표 위치에 위치시킴
					RoomNum = 1;
					CurrentState = ELocationState::MOVE;
				}
				else
				{
					dir.Normalize();
					SetActorLocation(GetActorLocation() + dir * Speed * DeltaTime); // 위치 업데이트
				}
			}
		}
	}


	// 4.98초마다 AILevel에 있는 RandomMove() 호출 && Move로 상태전이
	CurrentTime += DeltaTime;

	if (CurrentTime > MovableTime) // 이동 가능한 시간이 되면
	{
		// RandomMove가 true일 때만 move
		if (AILevelComp->RandomMove(this, DeltaTime) == true)
		{
			if (RoomNum != 5 && RoomNum != 6 && RoomNum != 9) // room5, room6이 아닐 때는 Move()
			{
				// 상세 조건들은 MOVE에 있음
				CurrentState = ELocationState::MOVE;
			}
			//////////////////////////////////////////////////////////////////////////////
			// room5 -> (손전등만 room1)
			//else if (RoomNum == 5)
			//{
				//PlayFootStepsSound();
				//this->GetMesh()->SetVisibility(true);

				////	만약, 손전등 ON -> room1로 이동 (순간이동X)
				//if (bIsFlashlightOn == true)
				//{
				//	StopFootStepsSound();
				//	
				//	dir = TagArr[1] - GetActorLocation();
				//	float Distance = dir.Size();

				//	if ( Distance < 100.f )
				//	{
				//		SetActorLocation(TagArr[1]); // 정확히 목표 위치에 위치시킴
				//		RoomNum = 1;
				//		CurrentState = ELocationState::MOVE;
				//	}
				//	else
				//	{
				//		dir.Normalize();
				//		SetActorLocation(GetActorLocation() + dir * Speed * DeltaTime); // 위치 업데이트
				//	}
				//}
			//}
			//////////////////////////////////////////////////////////////////////////////
			// room6 -> (손전등만 room1)
			//else if (RoomNum == 6)
			//{
			//	PlayFootStepsSound();
			//	this->GetMesh()->SetVisibility(true);

			//	//	만약, 손전등 ON -> room1로 이동 (순간이동X)
			//	if ( bIsFlashlightOn == true )
			//	{
			//		StopFootStepsSound();

			//		dir = TagArr[1] - GetActorLocation();
			//		float Distance = dir.Size();

			//		if ( Distance < 100.f )
			//		{
			//			SetActorLocation(TagArr[1]); // 정확히 목표 위치에 위치시킴
			//			RoomNum = 1;
			//			CurrentState = ELocationState::MOVE;
			//		}
			//		else
			//		{
			//			dir.Normalize();
			//			SetActorLocation(GetActorLocation() + dir * Speed * DeltaTime); // 위치 업데이트
			//		}
			//	}
			//}
			//////////////////////////////////////////////////////////////////////////////
			// room9 일 때
			else if (RoomNum == 9)
			{
				CurrentState = ELocationState::CLOSET;
			}
		}
		else
		{
			AILevelComp->RandomMove(this, DeltaTime);
		}
		CurrentTime = 0.f;
	}
}
//--------------------------------------------------------------------------------------

void AFoxy::Move()
{
	UE_LOG(LogTemp, Warning, TEXT("Foxy Move()"));
	FVector CurrentLocation = this->GetActorLocation();
	// 폭시 위치가 room number 몇 인지

	AFreddyPlayer* FreddyPlayer = Cast<AFreddyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld() , 0));
	AFreddyPlayer::LookAt LookState;
	LookState = FreddyPlayer->GetLookAtState();

	// room1 -> room2, room3가능
	if (RoomNum == 1)
	{
		StopFootStepsSound(); // 발소리 안 들리게

		TArray<int32> RoomTags = { 2, 3 };
		int32 RandomIndex = FMath::RandRange(0 , RoomTags.Num() - 1);

		if ( RoomTags[RandomIndex] == 2 )
		{
			// 플레이어가 왼쪽 문에 있고 문이 열려있을 때에는 안 움직이도록 설정
			if ( LookState != AFreddyPlayer::LookAt::Left || bIsDoorClose == true )
			{
				SetActorLocation(TagArr[2]);
				RoomNum = 2;
			}
		}
		else if ( RoomTags[RandomIndex] == 3 )
		{
			// 플레이어가 오른쪽 문에 있고 문이 열려있을 때에는 안 움직이도록 설정
			if ( LookState != AFreddyPlayer::LookAt::Right || bIsDoorClose == true )
			{
				SetActorLocation(TagArr[3]);
				RoomNum = 3;
			}
		}
		
	}
	// room2 -> room3, room5 가능
	else if (RoomNum == 2)
	{
		PlayFootStepsSound(); // 발소리

		TArray<int32> RoomTags = { 3, 5 };
		int32 RandomIndex = FMath::RandRange(0, RoomTags.Num() - 1);

		if ( RoomTags[RandomIndex] == 5 )
		{
			// 플레이어가 왼쪽 문에 있고 문이 열려있을 때에는 안 움직이도록 설정
			if ( LookState != AFreddyPlayer::LookAt::Left || bIsDoorClose == true )
			{
				SetActorLocation(TagArr[5]);
				RoomNum = 5;
			}
		}
		else if ( RoomTags[RandomIndex] == 3 )
		{
			// 플레이어가 오른쪽 문에 있고 문이 열려있을 때에는 안 움직이도록 설정
			if ( LookState != AFreddyPlayer::LookAt::Right || bIsDoorClose == true )
			{
				SetActorLocation(TagArr[3]);
				RoomNum = 3;
			}
		}
	}
	// room3 -> room2, room6 가능
	else if (RoomNum == 3)
	{
		PlayFootStepsSound(); // 발소리

		TArray<int32> RoomTags = { 2, 6 };
		int32 RandomIndex = FMath::RandRange(0, RoomTags.Num() - 1);

		if ( RoomTags[RandomIndex] == 2 )
		{
			// 플레이어가 왼쪽 문에 있고 문이 열려있을 때에는 안 움직이도록 설정
			if ( LookState != AFreddyPlayer::LookAt::Left || bIsDoorClose == true )
			{
				SetActorLocation(TagArr[2]);
				RoomNum = 2;
			}
		}
		else if ( RoomTags[RandomIndex] == 6 )
		{
			// 플레이어가 오른쪽 문에 있고 문이 열려있을 때에는 안 움직이도록 설정
			if ( LookState != AFreddyPlayer::LookAt::Right || bIsDoorClose == true )
			{
				SetActorLocation(TagArr[6]);
				RoomNum = 6;
			}
		}
	}
	
	CurrentState = ELocationState::IDLE;
}

//--------------------------------------------------------------------------------------
void AFoxy::Attack()
{
	// 테스트용 -> 카메라 앞으로 SetActorLocation
	AFreddyPlayer* FreddyPlayer = Cast<AFreddyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FTransform JmpScare = FreddyPlayer->GetCameraTransform();
	JmpScare.SetLocation(JmpScare.GetLocation() - FVector(0 , 100 , 60)); // 위치 조정
	FRotator rot = JmpScare.GetRotation().Rotator();
	rot = FRotator(7.84f , 10.12f , 13.63f);
	JmpScare.SetRotation(rot.Quaternion());
	SetActorTransform(JmpScare); // 카메라 위치로 이동 (점프스케어)
/*	this->SetActorRotation(FRotator(7.84f , 10.12f , 13.63f));*/

	if ( bJSound == false )
	{
		// 점프스퀘어 anim 재생
		FoxyAnimInstance->IsJumpScare = true;
		PlayJumpScare();

		// 점프스케어 소리 재생
		UGameplayStatics::PlaySound2D(this , JumpScareSFX);
		bJSound = true;

		FreddyPlayer->OnDie(TEXT("Foxy"));

		UE_LOG(LogTemp , Warning , TEXT("Foxy Attack !"));
	}

	// 게임 오버

}
//--------------------------------------------------------------------------------------
void AFoxy::PlayJumpScare()
{
	if ( FoxyAnimInstance )
	{
		FoxyAnimInstance->PlayJumpScareMontage();
	}
}

void AFoxy::PlayFakeScare()
{
	if ( FoxyAnimInstance )
	{
		FoxyAnimInstance->PlayFakeScareMontage();
	}
}

FVector AFoxy::FindActorsWithTag(FName Tag)
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

void AFoxy::CanMove()
{
	RoomNum = 1;

	SetActorLocation(TagArr[1]);
	CurrentState = ELocationState::IDLE;
}

void AFoxy::FlashOn()
{
	AFreddyPlayer* FreddyPlayer = Cast<AFreddyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (FreddyPlayer)
	{
		bIsFlashlightOn = FreddyPlayer->GetFlash();
	}
}

void AFoxy::DoorOpen()
{
	AFreddyPlayer* FreddyPlayer = Cast<AFreddyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (FreddyPlayer)
	{
		bIsDoorClose = FreddyPlayer->GetrCloseDoor();
	}
}


void AFoxy::Closet(float DeltaTime)
{
	// 플레이어
	AFreddyPlayer* FreddyPlayer = Cast<AFreddyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld() , 0));
	AFreddyPlayer::LookAt LookState;
	LookState = FreddyPlayer->GetLookAtState();

	// 폭시 인형
	AActor* FoxDollInstance = UGameplayStatics::GetActorOfClass(GetWorld() , AFoxDoll::StaticClass());

	//if ( bIsFoxy == true )
	//{
	//	ShowFoxy(MeshState3, true);
	//	ShowFoxyDoll(FoxDollInstance , false); // 폭시 들어왔을 땐 안 보이게
	//}
	//else
	//{
	//	this->GetMesh()->SetVisibility(false); // skeletal 안 보이게
	//	ShowFoxy(MeshState3 , false); // 인형 상태일 땐 폭시 안 보이게
	//	ShowFoxyDoll(FoxDollInstance , true);
	//}

	if ( FreddyPlayer )
	{
		StopFootStepsSound(); // 발소리 안 들리게
		// 플레이어 위치 == 가운데, 옷장이 살짝 움직임 anim

		// 3단계면 점프스케어 가능
		if ( FoxyState == 3 && bIsDoorClose == false )
		{
			// 3초 후 점프스케어 (공격) → GAME OVER
			CurrentTime += DeltaTime;
			if ( CurrentTime > 3.f )
			{
				bAttack = true;
				CurrentTime = 0.f; // 초기화
			}
			// 메인으로 갔을 때 점프스케어
		}

		// 플레이어 위치 != CLOSET
		if ( LookState != AFreddyPlayer::LookAt::Center )
		{
			// 밖에선 둘 다 안 보이게
			FoxyMeshComponent->SetHiddenInGame(true);
			this->GetMesh()->SetVisibility(false);
			ShowFoxyDoll(FoxDollInstance , false);
		
			// 옷장에서 점프스케어 조건이 찼으면 메인에 가면 점프스케어
			if ( LookState == AFreddyPlayer::LookAt::Main )
			{
				ScareCount = 0.f; // 점프스케어 카운트 초기화

				if ( bAttack == true )
				{
					this->GetMesh()->SetVisibility(true); // skeletal 보이게
					if ( FreddyPlayer->KeepJumpScare() == false ) 
					{ 
						CurrentState = ELocationState::ATTACK; 
						Attack();
					}
					return;
				}

				if ( bClosetAnim == false )
				{

					bClosetAnim = true;
					// 옷장 움직이는 anim(Loop안 함)
					UE_LOG(LogTemp , Log , TEXT("Closet door Move !!"));

					CurrentState = ELocationState::IDLE; // 아래 if를 실행하기 위함
				}
			}
		}

		// 플레이어 위치 == CLOSET
		else if ( LookState == AFreddyPlayer::LookAt::Center )
		{
			ScareCount = 0.f; // 점프스케어 카운트 초기화

			if ( bCTtoZero == true ) // 반복적으로 currentTime이 0이 되지 않도록
			{
				CurrentTime = 0.f;
				bCTtoZero = false;
			}
			// 옷장 문 닫았을 때 (폭시 사라질 시간동안), 그 자리에 인형 스폰 (프레디처럼 이제 동작), 폭시는 다른 데로 안 가고 상태변화만 함

			// 3단계 -> 인형 구간
			if( StateToFoxy == false ) 
			{
				//// 3단계면 점프스케어 가능
				//if ( FoxyState == 3 && bIsDoorClose == false)
				//{
				//	// 3초 후 점프스케어 (공격) → GAME OVER
				//	CurrentTime += DeltaTime;
				//	if ( CurrentTime > 3.f )
				//	{
				//		bAttack = true;
				//		CurrentTime = 0.f; // 초기화
				//	}
				//	// 메인으로 갔을 때 점프스케어
				//}
				// 문 닫을 때마다 StateCount 감소 (3초 감소하면 State 변하게)  
				if ( bIsDoorClose == true )
				{
					StateCount -= DeltaTime;
					if ( StateCount < -3.f )
						FoxyState--;
				}
			}
			// 인형 -> 3단계 구간
			else if (StateToFoxy == true)
			{	
				// 문 열릴 때마다 StateCount 증가 
				if ( bIsDoorClose == false )
				{
					StateCount += DeltaTime;
					if ( StateCount > 3.f )	{ FoxyState++;}

					// 3단계면 점프스케어 가능
					if ( FoxyState == 4 ) // 4단계를 점프스케어로
					{
						// 3초 후 점프스케어 (공격) → GAME OVER
						bAttack = true;
						// 메인으로 갔을 때 점프스케어
					}
				}
			}
			/////////////////////////////////////////////////////////////////
			if ( FoxyState == 3 && this->GetActorLocation().Equals(TagArr[9] , 0.1f) )
			{
				// 폭시 인형은 안 보이게
				ShowFoxyDoll(FoxDollInstance, false); 
				
				// 불 켜면 페이크 점프스케어 anim, 한 번 나온 뒤엔 정지상태 mesh
				if ( bIsFlashlightOn == true )
				{
					if ( bFake == false && bWasFlashlightOn == false)
					{
						// 페이크 점프스케어 재생
						this->GetMesh()->SetVisibility(true); // skeletal 보이게
						FoxyMeshComponent->SetHiddenInGame(true); // 멈춘 버전 mesh는 안 보이게
						FoxyAnimInstance->IsFakeScare = true;
						PlayFakeScare();
						bFake = true;
					}
					else if ( bFake && bWasFlashlightOn )
					{
						// 애니메이션이 계속 재생되도록
						FoxyAnimInstance->IsFakeScare = true;
					}
				}
				else
				{
					// 불이 꺼졌을 때
					if ( bWasFlashlightOn )
					{
						// 애니메이션을 멈추고 멈춘 버전 Mesh 적용
						FoxyAnimInstance->IsFakeScare = false;
						//this->GetMesh()->SetVisibility(false); // Skeletal Mesh는 안 보이게
						ShowFoxy(MeshState3 , true);
						FoxyMeshComponent->SetRelativeLocation(FVector(0 , 0 , 90));

						bFake = false; // 다시 불을 켰을 때 애니메이션이 재생되도록 초기화
					}
				}
				// 현재 불이 켜져 있는 상태 저장
				bWasFlashlightOn = bIsFlashlightOn;
			}
			else if ( FoxyState == 2 && this->GetActorLocation().Equals(TagArr[9] , 0.1f) )
			{
				// 폭시 인형은 안 보이게
				ShowFoxyDoll(FoxDollInstance , false);
				// 허리 구부리고 얼굴 약간 보이는 mesh 적용
				FoxyMeshComponent->SetStaticMesh(MeshState2);
				FoxyMeshComponent->SetRelativeLocation(FVector(0 , 0 , 0));
			}
			else if ( FoxyState == 1 && this->GetActorLocation().Equals(TagArr[9] , 0.1f) )
			{
				// 폭시 인형은 안 보이게
				ShowFoxyDoll(FoxDollInstance , false);
				// 오른쪽에 서 있고 갈고리 손만 보이는 mesh 적용
				FoxyMeshComponent->SetStaticMesh(MeshState1);
				FoxyMeshComponent->SetRelativeLocation(FVector(0 , 0 , 90));
			}
			else if ( FoxyState == 0 && this->GetActorLocation().Equals(TagArr[9] , 0.1f) )
			{
				// 인형 어셋 적용
				//bIsFoxy = false; // Tick에서 CLOSET불러와서 ShowFoxyDoll 처리해 줄 것

				StateToFoxy = true;
				FoxyMeshComponent->SetHiddenInGame(true);
				ShowFoxyDoll(FoxDollInstance, true);
				UE_LOG(LogTemp , Log , TEXT("Spawn Foxy Doll"));
			}
		}
	}
}

void AFoxy::ShowFoxy(UStaticMesh* mesh , bool bShow)
{
	// mesh 컴포넌트로 안 보이게 (처음엔 보이게, room9에서 인형으로 바뀌면 안 보이게)
	FoxyMeshComponent->SetStaticMesh(mesh);
//	FoxyMeshComponent->SetRenderInMainPass(bShow);
	FoxyMeshComponent->SetHiddenInGame(!bShow);
	if ( bShow )
	{
		FoxyMeshComponent->SetHiddenInGame(false);
		this->GetMesh()->SetVisibility(false);  // 스켈레탈 메쉬 숨김
	}
	else
	{
		FoxyMeshComponent->SetHiddenInGame(true);
		this->GetMesh()->SetVisibility(true);  // 스켈레탈 메쉬 표시
	}
}

void AFoxy::ShowFoxyDoll(AActor* actor, bool bShow)
{
	AFoxDoll* FoxyDoll = Cast<AFoxDoll>(actor);

	if (FoxyDoll && FoxyDoll->FoxyDollComp)
	{
//		FoxyDoll->FoxyDollComp->SetRenderInMainPass(bShow);
		FoxyDoll->FoxyDollComp->SetHiddenInGame(!bShow);
	}
}

void AFoxy::PlayFootStepsSound()
{
	if ( FootStepsSFX && !FootStepsAudioComponent->IsPlaying() )
	{
		FootStepsAudioComponent->SetSound(FootStepsSFX);
		FootStepsAudioComponent->Play();
	}
}

void AFoxy::StopFootStepsSound()
{
	if ( FootStepsAudioComponent->IsPlaying() )
	{
		FootStepsAudioComponent->Stop();
	}
}