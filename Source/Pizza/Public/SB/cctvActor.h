// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "cctvActor.generated.h"

UCLASS()
class PIZZA_API AcctvActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AcctvActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;

	//UPROPERTY(EditDefaultsOnly)
	//class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	class UTimelineComponent* TimelineComp;
	
	UFUNCTION()
	void HandleProgress(float Value);

	UPROPERTY(EditDefaultsOnly)
	class UCurveFloat* CurveFloat;

	FVector InitialLocation;
};
