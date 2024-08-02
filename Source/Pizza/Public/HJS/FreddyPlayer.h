// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FreddyPlayer.generated.h"

UCLASS()
class PIZZA_API AFreddyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFreddyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	enum class LookAt
	{
		Left,
		Center,
		Right,
		Main,
		Bed,
		Back,
	};
	enum class CameraMove
	{
		LeftBoost,
		Left,
		Stop,
		Right,
		RightBoost,
	};
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 특정 문으로 이동하도록 세팅
	void SetMoveDoor(int32 DoorNum);

	// SetDown 함수의 가동을 ON 시켜주는 함수
	void SetUp();

	// 뒤로 돌기 & 뒤로 돌아가기
	void SetDown();

	// 불 켰는 지 껐는 지 리턴
	bool GetFlash();

	// 문 닫았는 지, 열었는 지 리턴
	bool GetrCloseDoor();

	// 현재 보고 있는 위치 리턴
	LookAt GetLookAtState();

	FTransform GetCameraTransform();

private:

	// 인핸스드 인풋
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputMappingContext* PlayerInputContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* FlashAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* CloseDoorAction;

	// 인풋 매핑 함수
	UFUNCTION()
	void OnFlash();
	UFUNCTION()
	void OffFlash();
	UFUNCTION()
	void CloseDoor();
	UFUNCTION()
	void OpenDoor();

	// 스프링암 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category="Camera")
	class USpringArmComponent* SpringArmComp;
	// 카메라 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
    class USplineComponent* SplineComponent1;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
    USplineComponent* SplineComponent2;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
    USplineComponent* SplineComponent3;

	// 3개의 스플라인 배열
	TArray<USplineComponent*> Splines;

	// 플레이어의 현재 위치 State
	LookAt LookAtState = LookAt::Main;

	// 플레이어의 손전등 OnOff 여부
	bool bFlash = false;

	// 플레이어의 문 닫았는 지 여부
	bool bClose = false;

	// 마우스 입력 상태 변수
	bool bAllowBack = true;

	// 이동 중인 지 상태 변수
	bool bMoving = false;

	bool bReverse = false;

	// 특정 문으로 이동
	void Move(float DeltaTime);

	UPROPERTY(EditAnywhere,  Category = "Movement")
    float MovementSpeed = 10.f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float PitchAmount = 50.f;

	float CurrentTime = 0.0f;

	UFUNCTION()
	void SetTurnState();
	// 카메라 움직임 상태 Enum

	// 카메라 상태 State 변수
	CameraMove CameraState = CameraMove::Stop;
	//int32 CameraState = CameraMove::LeftBoost;
	// 마우스 위치를 저장할 변수
	FVector2D MousePosition;
	// 카메라 상태를 저장하고 있는 배열
	TArray<CameraMove> CameraStates;

	// 카메라 최대 이동 각도 설정
	UPROPERTY(EditAnywhere, Category="Custom")
	float CameraMaxAngle = 20.f;
	// 스포트라이트 최대 이동 각도 설정
	UPROPERTY(EditAnywhere, Category = "Custom")
	float LightMaxAngle = 40.f;

	// 손전등 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Light")
	class USpotLightComponent* FlashlightComp;

	// 켜고 끄는 손전등 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Light")
	class USpotLightComponent* HandlightComp;

	// 손전등 위치를 업데이트하는 함수
	void UpdateFlashlight(float DeltaTime);

	// 뒤를 돌아보는 함수
	void LookBack(float DeltaTime);

	// 카메라를 돌리는 함수
	void CameraTurn(float DeltaTime);

	// 카메라 이동 속도
	UPROPERTY(EditDefaultsOnly, Category = "Custom")
	float RotationSpeed = 50.f; // 기본 회전 속도

	UPROPERTY(EditDefaultsOnly, Category = "Custom")
	float BoostSpeed = 2.f; // 부스트 속도 배율

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDownMouseUI> DownMouseUIFactory;

	UPROPERTY()
	class UDownMouseUI* DownMouseUI;

	// 이동시 머리를 숙이고 드는 기능
	
	bool bHeadDown = false;
	bool bHeadUp = false;
	float HeadMovementTime = 0.5f; // 고개를 숙이고 드는 시간
	float HeadCurrentTime = 0.0f;

	void UpdateHeadMovement(float DeltaTime);

	// 걸어가는 카메라 쉐이크
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCameraShakeBase> WalkShake;

	// 위치 도착 후 문 열면서 몸 기울이기
	// 문 변수 3개 흠... 배열?  LookAt으로 문 배열에 접근
	UPROPERTY(VisibleAnywhere)
	TArray<class ADoor*> Doors;
	// 문 열면서 고개 기울이는 함수
	void DoorRotAndCameraMove(float DeltaTime);

	// 
	int32 CloseBoost = 5;
	bool bCloseDoor = false;
	// 추가할 변수들
	bool bOpenDoor=false;
	int32 DoorIndex=-1;
	FRotator DoorRotation;
	FVector CameraOffset;
	FRotator CameraRotation;
	void SetUpdateDoor(int32 DoorNum);
	void SetBackDoor(int32 BackNum);
	UPROPERTY(EditAnywhere)
	USceneComponent* LeftDoorMovePoint;
	UPROPERTY(EditAnywhere)
	USceneComponent* RightDoorMovePoint;

	UPROPERTY(EditAnywhere)
	USceneComponent* RightBackMovePoint;

	UPROPERTY(EditAnywhere)
	USceneComponent* LeftBackMovePoint;

	UPROPERTY(EditAnywhere)
	USceneComponent* CenterBackMovePoint;

	FVector OriginCameraVector;
	FRotator OriginCameraRotate;


	// 문 닫기/열기 로테이트는 기존 변수 재사용
	// 문 닫기/열기 완료 스테이트
	bool bCompleteOpenOrClose;
	// 카메라 이동 위치를 저장할 MovePoint
	UPROPERTY(EditAnywhere)
	float LeftDoorClosePoint = 0.f;
	UPROPERTY(EditAnywhere)
	float LeftDoorOpenPoint = -35.f;
	UPROPERTY(EditAnywhere)
	float LeftDoorOriginPoint = -13.f;
	UPROPERTY(EditAnywhere)
	float RightDoorClosePoint = 0.f;
	UPROPERTY(EditAnywhere)
	float RightDoorOriginPoint = 13.f;

	UPROPERTY(EditAnywhere)
	float RightDoorOpenPoint = 40.f;

	UPROPERTY(EditAnywhere)
	USceneComponent* LeftCameraClosePoint;
	UPROPERTY(EditAnywhere)
	USceneComponent* RightCameraClosePoint;

	UPROPERTY(EditAnywhere)
	USceneComponent* CenterDoorClosePoint;
	UPROPERTY(EditAnywhere)
	USceneComponent* CenterDoorOpenPoint;
};
