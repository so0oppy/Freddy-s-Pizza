// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JYS/AllEnemyCharacter.h"
#include "EnemyBonnie.generated.h"

enum class EBonnieState : uint8
{
	Room0,
	Room1,
	Room2,
	Room3,

};

UCLASS()
class PIZZA_API AEnemyBonnie : public AAllEnemyCharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBonnie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// EBonnieState State = EBonnieState::Room3;

	void Move(EBonnieState MoveState);

private:
	// 방을 배열로 구현
	FVector RoomPositions[4];

	// 이동 구현
	EBonnieState State;

	FTimerHandle MoveTimerHandle;
	
	void TickRoom0(const float& DeltaTime);
	void TickRoom1(const float& DeltaTime);
	void TickRoom2(const float& DeltaTime);
	void TickRoom3(const float& DeltaTime);

	// 이동 및 랜덤 숫자 뽑기
	UFUNCTION()
	void AttemptMove();
	UFUNCTION()
	int32 GetRandomNumber();
};
