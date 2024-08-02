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

	// ť��(Freddy) ���� �迭
	TArray<USkeletalMeshComponent*> FreddysArr;

	// ť��(Freddy)�� ���� �Ǵ� Ÿ�̸�
	FTimerHandle FreddysVisibleTimerHandle;

	// ť��(Freddy)�� ���� �Ǵ� �Լ�
	void AttemptSpawnCube();
	
	// ť��(Freddy) Hide
	void HideFreddy(float DeltaTime);

	// +1�� 
	float HiddenTime = 0;

	// ���� ���� ����� �Լ�
	int32 GetRandomNumber();

	// Player�� Bed�� ���� �ִ��� üũ
	bool IsPlayerLookingAtBedAndFlashOn();


};
