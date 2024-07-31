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

}

void AFreddyPlayer::SetUp()
{
	// UP은 카메라 이동이 다시 가능한 상태로 만들기
	bAllowBack = true;
}

void AFreddyPlayer::SetDown()
{	
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
			bHeadDown = true;
			HeadCurrentTime = 0.0f;
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

AFreddyPlayer::LookAt AFreddyPlayer::GetLookAtState()
{
	return LookAtState;
}

void AFreddyPlayer::OnFlash()
{
	// 불 켜기
	HandlightComp->SetVisibility(true);
	bFlash = true;
}

void AFreddyPlayer::OffFlash()
{
	// 불 끄기
	HandlightComp->SetVisibility(false);
	bFlash = false;
}

void AFreddyPlayer::CloseDoor()
{
	// 문 닫기	
}

void AFreddyPlayer::OpenDoor()
{
	// 문 열기
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
			bOpenDoor=true;  // 문 회전을 시작
			DoorIndex=DoorNum;
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
		float Pitch = bHeadDown ? FMath::Lerp(0.0f, -80.0f, Alpha) : FMath::Lerp(-80.0f, 0.0f, Alpha);
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
	if ( !bOpenDoor || DoorIndex == -1 )
	{
		return;
	}

	ADoor* Door=Doors[DoorIndex];
	if ( Door )
	{
		FRotator NewRotation=FMath::RInterpTo(Door->GetActorRotation(), DoorRotation, DeltaTime, 2.0f);
		Door->SetActorRotation(NewRotation);
	}

	FVector NewCameraOffset=FMath::VInterpTo(SpringArmComp->GetRelativeLocation(), SpringArmComp->GetRelativeLocation() + CameraOffset, DeltaTime, 2.0f);
	SpringArmComp->SetRelativeLocation(NewCameraOffset);

	//FRotator NewCameraRotation=FMath::RInterpTo(SpringArmComp->GetRelativeRotation(), SpringArmComp->GetRelativeRotation()+CameraRotation, DeltaTime, 2.0f);
	//SpringArmComp->SetRelativeRotation(NewCameraRotation);

	// 목표 위치와 회전각에 도달했는지 확인
	//NewCameraRotation.Equals(CameraRotation, 0.1f)
	if ( NewCameraOffset.Equals(SpringArmComp->GetRelativeLocation()+CameraOffset, 0.1f) && Door->GetActorRotation().Equals(DoorRotation, 0.1f) )
	{
		bOpenDoor=false;
		DoorIndex=-1;
	}
}

void AFreddyPlayer::SetUpdateDoor(int32 DoorNum)
{
	if ( DoorNum == 0 ) // Left door
	{
		ADoor* Door=Doors[DoorNum];
		if ( Door )
		{
			DoorRotation=Door->GetActorRotation();
			DoorRotation.Yaw-=12.0f;

			// 왼쪽으로 카메라 기울이기
			CameraOffset=FVector(-20.0f, 0.0f, 0.0f); // 원하는 값으로 설정
			CameraRotation=SpringArmComp->GetRelativeRotation();
			CameraRotation.Roll-=10.0f; // 원하는 값으로 설정
		}
	}
	else if ( DoorNum == 2 ) // Right door
	{
		ADoor* Door=Doors[DoorNum];
		if ( Door )
		{
			DoorRotation=Door->GetActorRotation();
			DoorRotation.Yaw+=12.0f;

			// 오른쪽으로 카메라 기울이기
			CameraOffset=FVector(20.0f, 0.0f, 0.0f); // 원하는 값으로 설정
			CameraRotation=SpringArmComp->GetRelativeRotation();
			CameraRotation.Roll+=10.0f; // 원하는 값으로 설정
		}
	}
}
