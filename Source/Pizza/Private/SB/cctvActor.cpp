#include "SB/cctvActor.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"

AcctvActor::AcctvActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	SetRootComponent(CameraComp);

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));
}

void AcctvActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (CurveFloat)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleProgress"));
		TimelineComp->AddInterpFloat(CurveFloat, ProgressFunction);
		TimelineComp->SetLooping(true);
		TimelineComp->PlayFromStart();
	}

	InitialLocation = GetActorLocation();
}

void AcctvActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AcctvActor::HandleProgress(float Value)
{
	FVector NewLocation = InitialLocation;
	NewLocation.X += Value;
	SetActorLocation(NewLocation);
}

