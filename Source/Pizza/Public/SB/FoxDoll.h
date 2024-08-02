// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoxDoll.generated.h"

UCLASS()
class PIZZA_API AFoxDoll : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoxDoll();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 테스트용 폭시 인형 -> 따로 만들기
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* FoxyDollComp;
};
