// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LocationState.h"
#include "GameFramework/Character.h"
#include "AILevel.h"
#include "Foxy.generated.h"

UCLASS()
class PIZZA_API AFoxy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFoxy();
	void SetUpLocation(ELocationState State, float DeltaTime);
	void UpdateState(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	class UAILevel* AILevelComp;

	TArray<FVector> TagArr;
	int32 RoomNum = 1;

	// 현 위치 태그 확인
	// FName GetCurrentLocationTag();
	
	void Idle(float DeltaTime);
	void Move();
	void Attack();


private:
	UPROPERTY(EditDefaultsOnly)
	ELocationState CurrentState;

	// 매 5.01초마다 이동 가능
	float CurrentTime = 0.f;
	float MovableTime = 5.01f;

	// 침대를 바라볼 때 쌓일 점프스케어 카운트
	float ScareCount = 0.f;

	FVector FindActorsWithTag(FName Tag);
	void MoveToTaggedLocation(int32 room);
	void CanMove();
	FTimerHandle Handle;

	// 컨트롤 키 입력 함수 -> 손전등 ON/OFF
	void FlashOn();
	// Shift 키 입력 함수 -> 문/옷장 Open/Close
	void DoorOpen();
	bool bIsFlashlightOn = false;
	bool bIsDoorClose = false;

	bool bClosetAnim = false; // 옷장 애니메이션 한 번만 실행되게 하는 플래그
	bool bCTtoZero = false; // currentTime이 한 번만 0이 되게 하는 플래그
	bool bAttack = false; // 공격 조건

	//bool bIsAtDoor = false; // 테스트용 변수
	void ShowFoxy(ACharacter* character);
	void ShowFoxyDoll(AActor* actor, bool bShow);
	bool bIsFoxy = true; // 폭시일 때(t), 인형일 때(f) 구분

	// 발소리
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* FootStepsSFX;
	UPROPERTY()
    UAudioComponent* FootStepsAudioComponent;
	
	// 점프스케어 소리
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* JumpScareSFX;

	bool bFSound = false; // 발소리 토글
	bool bBSound = false; // 숨소리 토글
	bool bJSound = false; // 점프스케어 소리 토글

	void PlayFootStepsSound();
	void StopFootStepsSound();

	UPROPERTY()
	class AFreddyPlayer* player;
};
