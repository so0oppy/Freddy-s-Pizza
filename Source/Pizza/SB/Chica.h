#pragma once

#include "CoreMinimal.h"
#include "LocationState.h"
#include "GameFramework/Character.h"
#include "Chica.generated.h"
UCLASS()
class PIZZA_API AChica : public ACharacter
{
	GENERATED_BODY()

public:
	AChica();

	void SetUpLocation(ELocationState State, float DeltaTime, FName Tag);

protected:
	virtual void BeginPlay() override;
	

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 현 위치 태그 확인
	FName GetCurrentLocationTag();
	
	void Idle(float DeltaTime, FName Tag);
	void Move(FName Tag);
	void Attack();
	void Cupcake();

private:
	UPROPERTY(EditDefaultsOnly)
	ELocationState CurrentState;

	UPROPERTY()
	class UAILevel* AILevelComp;

	// 매 4.98초마다 이동 가능
	float CurrentTime = 0.f;
	float MovableTime = 4.98f;

	void MoveToTaggedLocation(FName Tag);

	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	FTimerHandle Timer;
	// 컨트롤 키 입력 함수
	void OnCtrlKeyPressed();
	bool bIsFlashlightOn = false;
	bool bIsDoorOpen = true;
};
