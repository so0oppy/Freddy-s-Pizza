// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JYS/AllEnemyCharacter.h"
#include "EnemyFreddy.generated.h"

UCLASS()
class PIZZA_API AEnemyFreddy : public AAllEnemyCharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyFreddy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// AI Level
	int32 Level;

	// 큐브(Freddy)가 스폰 되는 타이머
	FTimerHandle CubeSpawnTimerHandle;

	// 큐브(Freddy)가 작아지는 타이머
	FTimerHandle ShrinkTimerHandle;

	// 플레이어 가져오기
	class AFreddyPlayer* Player;

	// 큐브(Freddy)가 스폰 되는 함수
	void AttemptSpawnCube();
	
	// 큐브(Freddy)가 작아지다가 사라지는 함수
	void ShrinkAndRemoveCubes();

	// 큐브(Freddy) 스폰 배열
	TArray<AActor*> SpawnedCubes;

	// 큐브(Freddy) 작아지는 순서 배열
	int32 ShrinkIndex;

	// 랜덤 숫자 고르는 함수
	int32 GetRandomNumber();

	// Player가 Bed를 보고 있는지 체크
	bool IsPlayerLookingAtBedAndFlashOn();

	// 큐브(Freddy)가 delay이후에 작아지는 함수
	void StartShrinkingCubes();

	// 큐브(Freddy)가 작아지기 전 delay
	const float SHRINK_DELAY = 2.0f;

};
