// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CupCake.generated.h"

UCLASS()
class PIZZA_API ACupCake : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACupCake();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 테스트용 컵케이크 큐브
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* CupCakeComp;
};
