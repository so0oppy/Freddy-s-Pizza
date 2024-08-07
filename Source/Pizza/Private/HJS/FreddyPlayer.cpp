// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/FreddyPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/SplineComponent.h"
#include "HJS/DownMouseUI.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Kismet/GameplayStatics.h"
#include "HJS/Door.h"
#include "HJS/HJSGameMode.h"
#include "SB/Chica.h"
#include "JYS/EnemyBonnie.h"
// Sets default values
AFreddyPlayer::AFreddyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SetRootComponent(GetMesh());

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	FlashlightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashlightComp"));
	FlashlightComp->SetupAttachment(RootComponent);
	FlashlightComp->Intensity = 5000.f; // 원하는 강도로 설정
	FlashlightComp->AttenuationRadius = 1000.f;

	HandlightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("HandLightComp"));
	HandlightComp->SetupAttachment(SpringArmComp);
	HandlightComp->Intensity = 5000.f; // 원하는 강도로 설정
	HandlightComp->AttenuationRadius = 1000.f;
	HandlightComp->SetVisibility(false);
	SplineComponent1 = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent1->SetupAttachment(RootComponent);
	
	SplineComponent2 = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent2"));
	SplineComponent2->SetupAttachment(RootComponent);

	SplineComponent3 = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent3"));
	SplineComponent3->SetupAttachment(RootComponent);

	LeftDoorMovePoint = CreateDefaultSubobject<USceneComponent>(TEXT("LeftDoorMovePoint"));
	LeftDoorMovePoint->SetupAttachment(RootComponent);
	RightDoorMovePoint=CreateDefaultSubobject<USceneComponent>(TEXT("RightDoorMovePoint"));
	RightDoorMovePoint->SetupAttachment(RootComponent);

	RightBackMovePoint=CreateDefaultSubobject<USceneComponent>(TEXT("RightBackMovePoint"));
	RightBackMovePoint->SetupAttachment(RootComponent);

	LeftBackMovePoint=CreateDefaultSubobject<USceneComponent>(TEXT("LeftBackMovePoint"));
	LeftBackMovePoint->SetupAttachment(RootComponent);

	CenterBackMovePoint=CreateDefaultSubobject<USceneComponent>(TEXT("CenterBackMovePoint"));
	CenterBackMovePoint->SetupAttachment(RootComponent);

	LeftCameraClosePoint=CreateDefaultSubobject<USceneComponent>(TEXT("LeftCameraClosePoint"));
	LeftCameraClosePoint->SetupAttachment(RootComponent);
	RightCameraClosePoint=CreateDefaultSubobject<USceneComponent>(TEXT("RightCameraClosePoint"));
	RightCameraClosePoint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFreddyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(true);

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetHideCursorDuringCapture(false);
		PlayerController->SetInputMode(InputMode);

		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (SubSystem)
		{
			SubSystem->AddMappingContext(PlayerInputContext, 0);
		}
	}
	
	Splines.SetNum(3);
	Splines[0] = SplineComponent1;
	Splines[1] = SplineComponent2;
	Splines[2] = SplineComponent3;

	if (DownMouseUIFactory)
	{
		DownMouseUI = Cast<UDownMouseUI>(CreateWidget(GetWorld(), DownMouseUIFactory, FName("DownMouseUI")));
		if (DownMouseUI)
		{
			DownMouseUI->AddToViewport();
			DownMouseUI->SetPlayer(this);
		}
	}

	// 문 태그로 찾아서 가져와서 배열에 하나씩 넣기
	Doors.SetNum(3);
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("LeftDoor"), FoundActors);
	if ( FoundActors.Num() > 0 )
	{
		 ADoor* LeftDoor = Cast<ADoor>(FoundActors[0]);
		 
		 if ( LeftDoor )
		 {
			 Doors[0] = LeftDoor;
		 }

	}

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("CenterDoor"), FoundActors);
	if ( FoundActors.Num() > 0 )
	{
		ADoor* CenterDoor=Cast<ADoor>(FoundActors[0]);

		if ( CenterDoor )
		{
			Doors[1]=CenterDoor;
		}
	}

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("RightDoor"), FoundActors);
	if ( FoundActors.Num() > 0 )
	{
		ADoor* RightDoor=Cast<ADoor>(FoundActors[0]);

		if ( RightDoor )
		{
			Doors[2]=RightDoor;
		}

	}
	//		(Pitch=6.336300,Yaw=0.000000,Roll=0.000000)
	OriginCameraRotate = FRotator(10.f, 0.f, 0.f);
	//(X=498.913821,Y=0.000000,Z=-140.000000)
	OriginCameraVector = FVector(499.f,0.f,-140.f);
}

void AFreddyPlayer::SetUp()
{
	// UP은 카메라 이동이 다시 가능한 상태로 만들기
	bAllowBack = true;
}

void AFreddyPlayer::SetDown()
{	
	if(bCompleteOpenOrClose || bClose == true)
	{
		return;
	}
	
	if ( bOpenDoor )
	{
		return;
	}
	if (bMoving)
	{
		return;
	}
	if (bAllowBack)
	{
		// UP하기 전에는 다시 불리더라도 이동이 안되게 만들기
		bAllowBack = false;
		// 카메라 이동
		// 
		// 메인에 있을 때는 카메라를 돌려서 침대로 이동하기
		// 카메라를 뒤로 돌릴건데..
		if (LookAtState == LookAt::Main)
		{
			// 뒤돌기
			LookAtState = LookAt::Bed;
			bMoving = true;
			CurrentTime = 0.f;
		}
		else if (LookAtState == LookAt::Bed)
		{
			LookAtState = LookAt::Back;
			bMoving = true;
			CurrentTime = 0.f;
		}
		else
		{
			// 문 앞이나 옷장에 있을때는 돌아가기 (스플라인 원상복구하면 될 듯)
			bReverse = true;
			CurrentTime = 0.f;
			bMoving = true;
			//bHeadDown = true;
			HeadCurrentTime = 0.0f;
			SetBackDoor(static_cast<int32>(LookAtState));
			if ( RunSound )
			{
				UGameplayStatics::PlaySound2D(GetWorld() , RunSound);
			}
			if (LookAtState != LookAt::Center && MoveCloseDoorSound )
			{
				UGameplayStatics::PlaySound2D(GetWorld() , MoveCloseDoorSound);
			}
		}
	}
}

bool AFreddyPlayer::GetFlash()
{
	return bFlash;
}

bool AFreddyPlayer::GetrCloseDoor()
{
	return bClose;
}

UCameraComponent* AFreddyPlayer::GetCameraComp()
{
	return CameraComp;
}

AFreddyPlayer::LookAt AFreddyPlayer::GetLookAtState()
{
	return LookAtState;
}

FTransform AFreddyPlayer::GetCameraTransform()
{
	return SpringArmComp->GetComponentTransform();
}

void AFreddyPlayer::OnDie()
{
	// 플레이어가 죽을 때
	// 플레이어 위치에 따른 카메라 쉐이크
	//if ( LookAtState == LookAt::Left || LookAtState == LookAt::Right )
	//{

	//}
	//else
	//{

	//}
	// 일단 통일, 쉐이크 깎기 너무 노가다 같음
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ( PlayerController )
	{
		PlayerController->ClientStartCameraShake(JumpScareShake1);
		// 플레이어 조작 멈추기 -> 틱을 꺼버리자
		SetActorTickEnabled(false);
		// 다른 Enemy 이어서 작동 안되도록 게임 Pause 시키기
		GetWorldTimerManager().SetTimer(PauseHandle,this,&AFreddyPlayer::OnMyPause,1.f,false);
		FInputModeGameAndUI InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->EnableInput(PlayerController);
		// 리스타트 모드 활성화 ( 이 상태에서 R키 누르면 리스타트 됨 )
		bEnableRestart = true;
	}

}

void AFreddyPlayer::OnRestart()
{
	if ( bEnableRestart)
	{
		UGameplayStatics::SetGamePaused(GetWorld() , false);
		FName CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld() , true));
		UGameplayStatics::OpenLevel(GetWorld() , CurrentLevelName);
	}
}

void AFreddyPlayer::OnMyPause()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ( PlayerController )
	{
		PlayerController->ClientStopCameraShake(JumpScareShake1);
		PlayerController->SetPause(true);
	}
	/*UGameplayStatics::SetGamePaused(GetWorld(), true);*/
}

void AFreddyPlayer::OnFlash()
{
	// 불 켜기
	HandlightComp->SetVisibility(true);
	bFlash = true;
	if ( LightSound )
	{
		UGameplayStatics::PlaySound2D(GetWorld() , LightSound);
	}
}

void AFreddyPlayer::OffFlash()
{
	// 불 끄기
	HandlightComp->SetVisibility(false);
	bFlash = false;
	if ( LightSound )
	{
		UGameplayStatics::PlaySound2D(GetWorld() , LightSound);
	}
}

void AFreddyPlayer::CloseDoor()
{
	if ( bMoving )
	{
		return;
	}
	// 아직 무브중이라면 return
	if ( bCloseDoor == true || bOpenDoor == true)
	{
		return;
	}
	
	// 현재 위치가 메인 혹은 배드라면 return
	if ( LookAtState == LookAt::Bed || LookAtState == LookAt::Main || LookAtState == LookAt::Back )
	{
		return;
	}

	
	AHJSGameMode* GM = Cast<AHJSGameMode>(GetWorld()->GetAuthGameMode());
	if ( GM != nullptr ) {
		// 오른쪽 문을 닫은 경우
		if ( LookAtState == LookAt::Right )
		{
			AChica* Chica = GM->GetChica();
			if ( Chica != nullptr )
			{
				if ( Chica->RoomNum == 6 )
				{
					bTeleport = true;
				}
			}
		}

		// 왼쪽 문을 닫은 경우

		if ( LookAtState == LookAt::Left )
		{
			AEnemyBonnie* Bonnie = GM->GetBonnie();
			if ( Bonnie != nullptr )
			{
				if ( Bonnie->State == EBonnieState::Room1 )
				{
					bTeleport = true;
				}
			}
		}
	}
	// 문 닫기 변수 변화
	bClose= true;
	
	// 문닫기
	// 1. 문의 Yaw를 목표 위치으로 만들기 ( 선형 보간 )
	// 2. 카메라를 뒤로 이동하기 ( 선형 보간 )
	// 1,2를 같은 함수에서 처리할 수 있도록 합치기
	// 필요 변수 문 : 목표 위치(센터), 목표 회전( 양쪽 문 ), 
	// 필요 변수 카메라 : 목표 위치 ( 양쪽 문에서는 카메라 이동, 센터에서는 이동 X)
	// 위치대로 이동시켜주는 틱 함수
	// 목표 위치 및 트리거를 ON 시키는 일 -> 여기서
	DoorIndex = static_cast<int32>(LookAtState);
	DoorRotation = Doors[DoorIndex]->GetActorRotation();
	DoorLocation = Doors[DoorIndex]->GetActorLocation();
	switch ( LookAtState )
	{
		case LookAt::Left:
		CameraOffset=LeftCameraClosePoint->GetRelativeLocation();
		DoorRotation.Yaw = LeftDoorClosePoint;
		if ( CloseDoorSound )
		{
			UGameplayStatics::PlaySound2D(GetWorld() , CloseDoorSound);
		}

		break;
		case LookAt::Right:
		CameraOffset=RightCameraClosePoint->GetRelativeLocation();
		DoorRotation.Yaw = RightDoorClosePoint;
		if ( CloseDoorSound )
		{
			UGameplayStatics::PlaySound2D(GetWorld() , CloseDoorSound);
		}
		break;
		case LookAt::Center:
		CameraOffset=SpringArmComp->GetRelativeLocation(); 
		DoorLocation.X = DoorMovePoint;
	}
	bCompleteOpenOrClose = true;

}

void AFreddyPlayer::OpenDoor()
{
	if ( bMoving )
	{
		return;
	}
	if ( bCloseDoor == true || bOpenDoor == true )
	{
		return;
	}

	if ( LookAtState == LookAt::Bed || LookAtState == LookAt::Main )
	{
		return;
	}
	// 문 열기 변수 변화
	bClose= false;
	bTeleport = false;

	DoorIndex=static_cast<int32>(LookAtState);
	DoorRotation=Doors[DoorIndex]->GetActorRotation();
	DoorLocation=Doors[DoorIndex]->GetActorLocation();
	switch ( LookAtState )
	{
	case LookAt::Left:
		CameraOffset=LeftDoorMovePoint->GetRelativeLocation();
		DoorRotation.Yaw=LeftDoorOpenPoint;
		if ( OpenDoorSound )
		{
			UGameplayStatics::PlaySound2D(GetWorld() , OpenDoorSound);
		}
		break;
	case LookAt::Right:
		CameraOffset=RightDoorMovePoint->GetRelativeLocation();
		DoorRotation.Yaw=RightDoorOpenPoint;
		if ( OpenDoorSound )
		{
			UGameplayStatics::PlaySound2D(GetWorld() , OpenDoorSound);
		}
		break;
	case LookAt::Center:
		CameraOffset=SpringArmComp->GetRelativeLocation();
		DoorLocation.X=DoorOriginPoint;
	}
	bCompleteOpenOrClose=true;
}

// Called every frame
void AFreddyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetTurnState();
	CameraTurn(DeltaTime);
	UpdateFlashlight(DeltaTime);
	Move(DeltaTime);
	LookBack(DeltaTime);
	UpdateHeadMovement(DeltaTime);
	DoorRotAndCameraMove(DeltaTime);
	GetCameraTransform();
	DoorOpenAndClose(DeltaTime);
}

// Called to bind functionality to input
void AFreddyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (Input)
	{
		if (FlashAction)
		{
			Input->BindAction(FlashAction, ETriggerEvent::Started, this, &AFreddyPlayer::OnFlash);
			Input->BindAction(FlashAction, ETriggerEvent::Completed, this, &AFreddyPlayer::OffFlash);
		}
		if (CloseDoorAction)
		{
			Input->BindAction(CloseDoorAction, ETriggerEvent::Started, this, &AFreddyPlayer::CloseDoor);
			Input->BindAction(CloseDoorAction, ETriggerEvent::Completed, this, &AFreddyPlayer::OpenDoor);
		}
		if ( RestartAction )
		{
			Input->BindAction(RestartAction, ETriggerEvent::Started , this , &AFreddyPlayer::OnRestart);
		}
	}
}

void AFreddyPlayer::SetMoveDoor(int32 DoorNum)
{
	// 무빙중이면 리턴
	if (bMoving)
	{
		return;
	}

	if (LookAtState != LookAt::Main)
	{
		return;
	}

	bReverse = false;
	LookAtState = static_cast<LookAt>(DoorNum);
	CurrentTime = 0.0f;
	bMoving = true;
	bHeadDown = true; // 이동 시작 시 고개를 숙이기 시작
	HeadCurrentTime = 0.0f;
	if ( RunSound )
	{
		UGameplayStatics::PlaySound2D(GetWorld() , RunSound);
	}
}

void AFreddyPlayer::Move(float DeltaTime)
{
	if (!bMoving)
	{
		return;
	}
	int32 DoorNum = 0;
	switch (LookAtState)
	{
	case LookAt::Left:
		DoorNum = 0;
		break;
	case LookAt::Center:
		DoorNum = 1;
		break;
	case LookAt::Right:
		DoorNum = 2;
		break;
	default:
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->ClientStartCameraShake(WalkShake);
	}
	CurrentTime += DeltaTime;
	float SplineLength = Splines[DoorNum]->GetSplineLength();
	float MoveAmount;
	if (bReverse)
	{
		MoveAmount = FMath::Clamp(1.f - (CurrentTime * MovementSpeed / SplineLength), 0.f, 1.f);
		if (MoveAmount == 0.f)
		{
			LookAtState = LookAt::Main;
			bHeadUp = true;
			bMoving = false;
			HeadCurrentTime = 0.0f;
			bCloseDoor = false;
			SpringArmComp->SetRelativeRotation(FRotator(-80.f,0.f,0.f));
			//SetBackDoor(4);
		}
	}
	else
	{
		MoveAmount = FMath::Clamp(CurrentTime * MovementSpeed / SplineLength, 0.f, 1.f);
		
		if (MoveAmount > 0.8f && bHeadUp == false)
		{
			bHeadUp = true;
			HeadCurrentTime = 0.0f;
		}
		
		if (MoveAmount == 1.f)
		{
			bMoving = false;
			SetUpdateDoor(DoorNum);
		}
	}
	FVector SplineLocation = Splines[DoorNum]->GetLocationAtTime(MoveAmount, ESplineCoordinateSpace::Local);
	//FRotator SplineRotation = Splines[DoorNum]->GetRotationAtDistanceAlongSpline(MoveAmount, ESplineCoordinateSpace::Local);
	
	SpringArmComp->SetRelativeLocation(SplineLocation);
}

void AFreddyPlayer::SetTurnState()
{
	// 만약 메인 상태가 아니면 카메라를 좌우로 움직이지 않음.
	if (LookAt::Main != LookAtState)
	{
		CameraState = CameraMove::Stop;
		return;
	}

	if (GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		// MousePosition 변수에 현재 커서 위치가 저장됨
		
		// 뷰포트 사이즈를 받아와서
		float ViewportWidth = 0.f;
		if (GEngine && GEngine->GameViewport)
		{
			FViewport* Viewport = GEngine->GameViewport->Viewport;
			if (Viewport)
			{
				ViewportWidth = Viewport->GetSizeXY().X;
			}
		}
		// 커서 위치에 따라 해당 영역의 상태를 설정 
		// Clamp를 이용하여 구분지은 후 State에 추가해주기
		int32 Region = FMath::Clamp(static_cast<int32>(MousePosition.X / (ViewportWidth / 5)), 0, static_cast<int32>(CameraMove::RightBoost));
		if (Region < static_cast<int32>(CameraMove::LeftBoost) || Region > static_cast<int32>(CameraMove::RightBoost))
		{
			UE_LOG(LogTemp,Warning,TEXT("Out of Range Enum Class!"));
			return;
		}
		CameraState = static_cast<CameraMove>(Region);
	}
}

void AFreddyPlayer::UpdateFlashlight(float DeltaTime)
{
	FRotator DesiredRotation = FlashlightComp->GetRelativeRotation();
	FRotator TempCameraRotation = SpringArmComp->GetRelativeRotation();
	
	if ( LookAtState == AFreddyPlayer::LookAt::Main )
	{
		FlashlightComp->SetHiddenInGame(false);
	}
	else
	{
		FlashlightComp->SetHiddenInGame(true);
	}
	
	float Yaw =TempCameraRotation.Yaw;
	
	if (Yaw <= -18.f && Yaw >= -CameraMaxAngle)
	{
		// 왼쪽 비춤
		DesiredRotation.Yaw = -LightMaxAngle; // 왼쪽으로 회전
	}
	else if (Yaw > -18.f && Yaw < 18.f)
	{
		// 중앙 비춤
		DesiredRotation.Yaw = 0.f; // 중앙으로 회전
	}
	else if (Yaw >= 18.f && Yaw <= CameraMaxAngle)
	{
		// 오른쪽 비춤
		DesiredRotation.Yaw = LightMaxAngle; // 오른쪽으로 회전
	}

	FlashlightComp->SetRelativeRotation(FMath::RInterpConstantTo(FlashlightComp->GetRelativeRotation(), DesiredRotation, DeltaTime, 200.f));
}

void AFreddyPlayer::LookBack(float DeltaTime)
{
	
	if (!bMoving)
	{
		return;
	}

	// Bed 상태면 메인으로 되돌리기
	FRotator NewRotation = SpringArmComp->GetRelativeRotation();

	if (LookAtState==LookAt::Bed)
	{
		NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw + RotationSpeed * 10 *BoostSpeed * DeltaTime, 0, 178);
		SpringArmComp->SetRelativeRotation(NewRotation);
		if (NewRotation.Yaw > 177)
		{
			bMoving = false;
		}
		if (NewRotation.Yaw == -180)
		{
			NewRotation.Yaw = 180;
			bMoving = false;
		}
	}
	// Main 상태면 Bed로 되돌리기 인데..
	else if (LookAtState == LookAt::Back)
	{
		NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw - RotationSpeed *10*BoostSpeed * DeltaTime, 0, 180);
		SpringArmComp->SetRelativeRotation(NewRotation);
		if (NewRotation.Yaw < 3)
		{
			bMoving = false;
			LookAtState = LookAt::Main;
		}
	}
}

void AFreddyPlayer::CameraTurn(float DeltaTime)
{
	
	FRotator NewRotation = SpringArmComp->GetRelativeRotation();

	switch (CameraState)
	{
	case CameraMove::LeftBoost:
		NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw - RotationSpeed * BoostSpeed * DeltaTime, -CameraMaxAngle, CameraMaxAngle);
		break;
	case CameraMove::Left:
		NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw - RotationSpeed * DeltaTime, -CameraMaxAngle, CameraMaxAngle);
		break;
	case CameraMove::Stop:	
		return;
	case CameraMove::Right:
		NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw + RotationSpeed * DeltaTime, -CameraMaxAngle, CameraMaxAngle);
		break;
	case CameraMove::RightBoost:
		NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw + RotationSpeed * BoostSpeed * DeltaTime, -CameraMaxAngle, CameraMaxAngle);
		break;
	}
	SpringArmComp->SetRelativeRotation(NewRotation);
}

void AFreddyPlayer::UpdateHeadMovement(float DeltaTime)
{
	if (bHeadDown || bHeadUp)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->ClientStopCameraShake(WalkShake);
		}
		HeadCurrentTime += DeltaTime;
		float Alpha = FMath::Clamp(HeadCurrentTime / HeadMovementTime, 0.0f, 1.0f);
		float Pitch = bHeadDown ? FMath::Lerp(OriginCameraRotate.Pitch , -80.0f, Alpha) : FMath::Lerp(-80.0f, OriginCameraRotate.Pitch, Alpha);
		FRotator NewRotation = SpringArmComp->GetRelativeRotation();
		NewRotation.Pitch = Pitch;
		SpringArmComp->SetRelativeRotation(NewRotation);
		
		if (Alpha >= 1.0f)
		{
			bHeadDown = false;
			bHeadUp = false;
		}
	}
}

void AFreddyPlayer::DoorRotAndCameraMove(float DeltaTime)
{
	if ( (!bOpenDoor && !bCloseDoor) || DoorIndex == -1 )
	{
		return;
	}

	float DoorRotateSpeed=25.f;
	float CameraRotateSpeed=18.f;
	float CameraOffsetSpeed=220.f;

	if ( bCloseDoor )
	{
		DoorRotateSpeed*=CloseBoost;
		CameraRotateSpeed*=CloseBoost;
		CameraOffsetSpeed*=CloseBoost;
	}

	ADoor* Door = nullptr;
	if ( DoorIndex < 3 )
	{
		Door=Doors[DoorIndex];
	}
	

	if ( Door )
	{
		FRotator NewRotation=FMath::RInterpConstantTo(Door->GetActorRotation(), DoorRotation, DeltaTime, DoorRotateSpeed);
		Door->SetActorRotation(NewRotation);
	}

	FVector NewCameraOffset=FMath::VInterpConstantTo(SpringArmComp->GetRelativeLocation(), CameraOffset, DeltaTime, CameraOffsetSpeed);
	SpringArmComp->SetRelativeLocation(NewCameraOffset);

	FRotator NewCameraRotation=FMath::RInterpConstantTo(SpringArmComp->GetRelativeRotation(), CameraRotation, DeltaTime, CameraRotateSpeed);
	SpringArmComp->SetRelativeRotation(NewCameraRotation);

	// 목표 위치와 회전각에 도달했는지 확인
	//
	if ( NewCameraOffset.Equals(CameraOffset, 0.1f)
		&& NewCameraRotation.Equals(CameraRotation, 0.1f))
	{
		bCloseDoor = false;
		bOpenDoor=false;
		DoorIndex=-1;
	}
}

void AFreddyPlayer::SetUpdateDoor(int32 DoorNum)
{
	bOpenDoor = true;
	DoorIndex=DoorNum;
	if ( DoorNum == 0 ) // Left door
	{
		ADoor* Door=Doors[DoorNum];
		if ( Door )
		{
			DoorRotation=Door->GetActorRotation();
			DoorRotation.Yaw=LeftDoorOpenPoint;

			// 왼쪽으로 카메라 기울이기
			CameraOffset=LeftDoorMovePoint->GetRelativeLocation(); // 원하는 값으로 설정
			CameraRotation=LeftDoorMovePoint->GetRelativeRotation();
			if ( MoveOpenDoorSound )
			{
				UGameplayStatics::PlaySound2D(GetWorld() , MoveOpenDoorSound);
			}
		}
	}
	else if ( DoorNum == 2 ) // Right door
	{
		ADoor* Door=Doors[DoorNum];
		if ( Door )
		{
			DoorRotation=Door->GetActorRotation();
			DoorRotation.Yaw=RightDoorOpenPoint;

			// 오른쪽으로 카메라 기울이기
			CameraOffset=RightDoorMovePoint->GetRelativeLocation(); // 원하는 값으로 설정
			CameraRotation=RightDoorMovePoint->GetRelativeRotation();
			if ( MoveOpenDoorSound )
			{
				UGameplayStatics::PlaySound2D(GetWorld() , MoveOpenDoorSound);
			}
		}
	}
	else
	{
		bOpenDoor=false;
	}
}

void AFreddyPlayer::SetBackDoor(int32 BackNum)
{
	bCloseDoor=true;
	
	// 1은 왼쪽일 때
	ADoor* Door;
	if ( BackNum == 0 )
	{
		Door = Doors[BackNum];
		DoorRotation=Door->GetActorRotation();
		DoorRotation.Yaw=LeftDoorOriginPoint;
		DoorIndex = BackNum;
		// 왼쪽으로 카메라 기울이기
		CameraOffset=LeftBackMovePoint->GetRelativeLocation(); // 원하는 값으로 설정
		CameraRotation=LeftBackMovePoint->GetRelativeRotation();
	}
	// 2는 오른쪽일 때
	else if ( BackNum == 2 )
	{
		Door=Doors[BackNum];
		DoorIndex=DoorIndex=BackNum;;
		DoorRotation=Door->GetActorRotation();
		DoorRotation.Yaw=RightDoorOriginPoint;

		// 왼쪽으로 카메라 기울이기
		CameraOffset=RightBackMovePoint->GetRelativeLocation(); // 원하는 값으로 설정
		CameraRotation=RightBackMovePoint->GetRelativeRotation();
	}// 3은 오른쪽일 때
	else if( BackNum == 1 )
	{	
		DoorIndex =BackNum;
		// 왼쪽으로 카메라 기울이기
		CameraOffset=CenterBackMovePoint->GetRelativeLocation(); // 원하는 값으로 설정
		CameraRotation=CenterBackMovePoint->GetRelativeRotation();
	}
	// 4는 메인일 때
	else if ( BackNum == 4 )
	{
		DoorIndex=BackNum;
		CameraOffset = OriginCameraVector;
		CameraRotation = OriginCameraRotate;
	}
}

void AFreddyPlayer::DoorOpenAndClose(float DeltaTime)
{

	if ( bCompleteOpenOrClose == false || DoorIndex == -1 )
	{
		return;
	}
	float DoorRotateSpeed=25.f;
	float CameraRotateSpeed=18.f;
	float CameraOffsetSpeed=220.f;
	float DoorOffsetSpeed=220.f;
	DoorRotateSpeed*=CloseBoost;
	CameraRotateSpeed*=CloseBoost;
	CameraOffsetSpeed*=CloseBoost;
	DoorOffsetSpeed*=CloseBoost;
	ADoor* Door=nullptr;
	if ( DoorIndex < 3 )
	{
		Door=Doors[DoorIndex];
	}


	if ( Door )
	{
		FRotator NewRotation=FMath::RInterpConstantTo(Door->GetActorRotation(), DoorRotation, DeltaTime, DoorRotateSpeed);
		Door->SetActorRotation(NewRotation);
		FVector NewDoorOffset=FMath::VInterpConstantTo(Door->GetActorLocation(), DoorLocation, DeltaTime, DoorOffsetSpeed);
		Door->SetActorLocation(NewDoorOffset);
		UE_LOG(LogTemp,Warning,TEXT("%s"), *Door->GetActorRotation().ToString());
		FVector NewCameraOffset=FMath::VInterpConstantTo(SpringArmComp->GetRelativeLocation(), CameraOffset, DeltaTime, CameraOffsetSpeed);
		SpringArmComp->SetRelativeLocation(NewCameraOffset);

		// 목표 위치와 회전각에 도달했는지 확인
//
		if ( NewCameraOffset.Equals(CameraOffset, 0.1f) )
		{	
			if ( DoorIndex == 1 ) {
				if ( NewDoorOffset.Equals(DoorLocation, 0.1f) ) {
					bCompleteOpenOrClose=false;
					DoorIndex=-1;
				}
			}else{
				if ( NewRotation.Yaw == Door->GetActorRotation().Yaw) {
					Door->SetActorRotation(DoorRotation);
					bCompleteOpenOrClose=false;
					DoorIndex=-1;
				}
			}
		}
	}

}
