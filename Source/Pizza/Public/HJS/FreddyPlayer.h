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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	// 스프링암 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category="Camera")
	class USpringArmComponent* SpringArmComp;
	// 카메라 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
	class UCameraComponent* CameraComp;

	UFUNCTION()
	void SetTurnState();
	// 카메라 움직임 상태 Enum
	enum class CameraMove
	{
		LeftBoost,
		Left,
		Stop,
		Right,
		RightBoost,
	};
	// 카메라 상태 State 변수
	CameraMove CameraState = CameraMove::Stop;
	//int32 CameraState = CameraMove::LeftBoost;
	// 마우스 위치를 저장할 변수
	FVector2D MousePosition;
	// 카메라 상태를 저장하고 있는 배열
	TArray<CameraMove> CameraStates;

	// 카메라 최대 이동 각도 설정
	UPROPERTY(EditAnywhere)
	float CameraMaxAngle = 20.f;
	// 스포트라이트 최대 이동 각도 설정
	UPROPERTY(EditAnywhere)
	float LightMaxAngle = 40.f;

	// 손전등 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Light")
	class USpotLightComponent* FlashlightComp;

	// 손전등 위치를 업데이트하는 함수
	void UpdateFlashlight(float DeltaTime);

	// 카메라를 돌리는 함수
	void CameraTurn(float DeltaTime);
	// 카메라 이동 속도
	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed = 50.f; // 기본 회전 속도

	UPROPERTY(EditDefaultsOnly)
	float BoostSpeed = 2.f; // 부스트 속도 배율
};
