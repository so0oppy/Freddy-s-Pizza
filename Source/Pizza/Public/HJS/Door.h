// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class PIZZA_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	int32 GetDoorNum();

private:
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	class UWidgetComponent* MoveButton;

	UPROPERTY(EditAnywhere)
	int32 DoorNum = -1;
};
