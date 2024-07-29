// Fill out your copyright notice in the Description page of Project Settings.


#include "SB/LocationState.h"

// Sets default values for this component's properties
ULocationState::ULocationState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULocationState::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULocationState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

