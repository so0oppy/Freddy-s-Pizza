// Fill out your copyright notice in the Description page of Project Settings.


#include "JYS/PlayerPawn.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* MouseController = Cast<APlayerController>(GetController());
	if (MouseController)
	{
		MouseController->bShowMouseCursor = true;
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraComp->SetWorldRotation(CurrentRotation);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerPawn::Turn);
}

void APlayerPawn::Turn(float Value)
{
	if (Value != 0.0f)
	{
		CurrentRotation.Yaw += Value;
	}
}

