// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/FreddyPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/SplineComponent.h"
#include "HJS/DownMouseUI.h"
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

	SplineComponent1 = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent1->SetupAttachment(RootComponent);
	
	SplineComponent2 = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent2"));
	SplineComponent2->SetupAttachment(RootComponent);

	SplineComponent3 = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent3"));
	SplineComponent3->SetupAttachment(RootComponent);
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

}

void AFreddyPlayer::SetUp()
{
	// UP은 카메라 이동이 다시 가능한 상태로 만들기
	bAllowBack = true;
}

void AFreddyPlayer::SetDown()
{
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


		}
		else
		{
			// 문 앞이나 옷장에 있을때는 돌아가기 (스플라인 원상복구하면 될 듯)
			bReverse = true;
			CurrentTime = 0.f;
			UE_LOG(LogTemp,Warning,TEXT("2134"));
		}

		

	}



}

bool AFreddyPlayer::GetFlash()
{
	return bFlash;
}

AFreddyPlayer::LookAt AFreddyPlayer::GetLookAtState()
{
	return LookAtState;
}

// Called every frame
void AFreddyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetTurnState();
	CameraTurn(DeltaTime);
	UpdateFlashlight(DeltaTime);
	Move(DeltaTime);
}

// Called to bind functionality to input
void AFreddyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFreddyPlayer::SetMoveDoor(int32 DoorNum)
{
	bReverse = false;
	LookAtState = static_cast<LookAt>(DoorNum);
	CurrentTime = 0.0f;
}

void AFreddyPlayer::Move(float DeltaTime)
{
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
	case LookAt::Main:
		return;
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
		}
	}
	else
	{
		MoveAmount = FMath::Clamp(CurrentTime * MovementSpeed / SplineLength, 0.f, 1.f);
	}
	FVector SplineLocation = Splines[DoorNum]->GetLocationAtTime(MoveAmount, ESplineCoordinateSpace::World);
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
	FRotator CameraRotation = SpringArmComp->GetRelativeRotation();
	float Yaw = CameraRotation.Yaw;

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
		break;
	case CameraMove::Right:
		NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw + RotationSpeed * DeltaTime, -CameraMaxAngle, CameraMaxAngle);
		break;
	case CameraMove::RightBoost:
		NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw + RotationSpeed * BoostSpeed * DeltaTime, -CameraMaxAngle, CameraMaxAngle);
		break;
	}
	SpringArmComp->SetRelativeRotation(NewRotation);
}
