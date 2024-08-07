// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HJSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API AHJSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHJSGameMode();
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// 시간이 흐르게 하고 싶다.
	void TimePass();
	// 당장 구현하지 않겠지만, 날짜가 지나는 함수
	void DayPass();
	// 시간에는 날짜, 시간이 있다.
	UPROPERTY(EditAnywhere)
	int32 Day = 3;
	UPROPERTY(EditAnywhere)
	int32 Hour = 0;
	// 시간이 지날 때 Enemy들의 AILevel이 바뀐다.
	// 시간이 바뀔 때마다 AI의 레벨을 변경시킨다.
	void SetAILevelEnemy();
	// 게임 시작 시 각각의 AI 객체를 찾아 저장해두고
	UPROPERTY()
	class AChica* Chica;
	UPROPERTY()
	class AEnemyBonnie* Bonnie;
	UPROPERTY()
	class AEnemyFreddy* Freddy;
	UPROPERTY()
	class AFoxy* Foxy;
	// 각각의 Enemy의 룸 정보를 받아와 UI로 띄워주기
	TArray<TArray<int32>> LV_freddy;
	TArray<TArray<int32>> LV_bonnie;

	// 타이머로 일정 시간마다 시간 흐르기
	FTimerHandle HourHandle;

	// 타이머로 
	UPROPERTY(EditAnywhere)
	float GamePlayRate = 30.f;

	// UI 띄우기
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMinimapUI> MinimapUIFactory;

	class UMinimapUI* MinimapUI;

};
