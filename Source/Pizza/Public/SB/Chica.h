#pragma once

#include "CoreMinimal.h"
#include "LocationState.h"
#include "GameFramework/Character.h"
#include "AILevel.h"
#include "Chica.generated.h"

UCLASS()
class PIZZA_API AChica : public ACharacter
{
	GENERATED_BODY()

public:
	AChica();

	void SetUpLocation(ELocationState State, float DeltaTime);
	void UpdateState(float DeltaTime);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;

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
	void Cupcake();

	// 애니메이션
	void PlayJumpScare();
	void PlayCupCakeJumpScare();

private:
	UPROPERTY(EditDefaultsOnly)
	ELocationState CurrentState;

	// 매 4.98초마다 이동 가능
	float CurrentTime = 0.f;
	float MovableTime = 4.98f;

	float CupCakeTimer = 0.f; // 1, 3일 때 0 초기화 8에서 쌓이기 시작, 6에서도 계속 쌓임
	float CupCakeTime = 8.f;

	FVector FindActorsWithTag(FName Tag);
	//void MoveToTaggedLocation(int32 room);
	void CanMove();
	FTimerHandle Handle;

	FVector dir;
	float Speed = 500.f;

	// 컨트롤 키 입력 함수 -> 손전등 ON/OFF
	void FlashOn();
	// Shift 키 입력 함수 -> 문/옷장 Open/Close
	void DoorOpen();
	bool bIsFlashlightOn = false;
	bool bIsDoorClose = false;

	//bool bIsAtDoor = false; // 테스트용 변수


	// 발소리
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* FootStepsSFX;
	UPROPERTY()
    UAudioComponent* FootStepsAudioComponent;
	// 숨소리
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* BreathSFX;
	UPROPERTY()
    UAudioComponent* BreathAudioComponent;
	// 점프스케어 소리
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* JumpScareSFX;
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* CupCakeScareSFX;

	bool bFSound = false; // 발소리 토글
	bool bBSound = false; // 숨소리 토글
	bool bJSound = false; // 점프스케어 소리 토글

	void PlayFootStepsSound();
	void StopFootStepsSound();
	void PlayBreathSound();
	void StopBreathSound();

	UPROPERTY()
	class AFreddyPlayer* player;

	// AnimInstance 설정
	UPROPERTY()
	class UChicaAnimInstance* ChicaAnimInstance;
};
