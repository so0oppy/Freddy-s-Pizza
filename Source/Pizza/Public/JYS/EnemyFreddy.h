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

	// JumpScare
	UFUNCTION()
	void JumpScareFreddy();

private:

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

	// 큐브(Freddy) 3개 배열
	TArray<USkeletalMeshComponent*> FreddysArr;

	// 큐브(Freddy) 스폰 타이머
	FTimerHandle FreddysVisibleTimerHandle;

	// 큐브(Freddy) 스폰 구현부
	void AttemptSpawnCube();
	
	// 큐브(Freddy) Hide
	void HideFreddy(float DeltaTime);

	// +1�� 
	float HiddenTime = 0;

	// 랜덤 넘버 가져오기
	int32 GetRandomNumber();

	// Player�� Bed�� ���� �ִ��� üũ
	bool IsPlayerLookingAtBedAndFlashOn();


	// 프레디 점프스케어 사운드
	UPROPERTY(EditAnywhere)
	class USoundBase* JumpScareFreddySFX;

	void JumpScareFreddySound();

	// 플레이어가 Main을 오래 보고 있으면 프레디의 스폰 타이머가 1.5초로 줄어들게
	bool AddFreddyFastly();
	
	float CurrentTime = 0;

	FTimerHandle FreddysVisibleFastlyTimerHandle;

	bool NotLookingAtTheDoor();

	bool SpawnFreddyTimerOnce = false;

};
