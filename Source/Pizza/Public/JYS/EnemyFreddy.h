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

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Freddy0;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* FreddyMesh0;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Freddy1;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* FreddyMesh1;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Freddy2;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* FreddyMesh2;

	// 큐브(Freddy) 스폰 배열
	TArray<USkeletalMeshComponent*> FreddysArr;


	// 큐브(Freddy)가 스폰 되는 타이머
	FTimerHandle FreddysVisibleTimerHandle;

	// 플레이어 가져오기
	class AFreddyPlayer* Player;

	// 큐브(Freddy)가 스폰 되는 함수
	void AttemptSpawnCube();
	
	// 큐브(Freddy) Hide
	void HideFreddy(float DeltaTime);

	float HiddenTime = 0;

	// 랜덤 숫자 고르는 함수
	int32 GetRandomNumber();

	// Player가 Bed를 보고 있는지 체크
	bool IsPlayerLookingAtBedAndFlashOn();


};
