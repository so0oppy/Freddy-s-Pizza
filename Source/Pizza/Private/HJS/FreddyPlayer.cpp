// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/FreddyPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"

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
}

// Called every frame
void AFreddyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetTurnState();
	CameraTurn(DeltaTime);
	UpdateFlashlight(DeltaTime);
}

// Called to bind functionality to input
void AFreddyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFreddyPlayer::SetTurnState()
{
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

	if (Yaw <= -12.f && Yaw >= -CameraMaxAngle)
	{
		// 왼쪽 비춤
		DesiredRotation.Yaw = -LightMaxAngle; // 왼쪽으로 회전
	}
	else if (Yaw > -12.f && Yaw < 12.f)
	{
		// 중앙 비춤
		DesiredRotation.Yaw = 0.f; // 중앙으로 회전
	}
	else if (Yaw >= 12.f && Yaw <= CameraMaxAngle)
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
