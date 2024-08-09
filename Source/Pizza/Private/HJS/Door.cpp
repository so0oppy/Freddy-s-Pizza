// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/Door.h"
#include "Components/WidgetComponent.h"
#include "HJS/MoveButtonUI.h"
// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	MoveButton = CreateDefaultSubobject<UWidgetComponent>(TEXT("MoveButton"));
	MoveButton->SetupAttachment(RootComponent);
	MoveButton->SetWidgetSpace(EWidgetSpace::Screen);

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	if (UUserWidget* UserWidget = MoveButton->GetUserWidgetObject())
	{
		if (UMoveButtonUI* Widget = Cast<UMoveButtonUI>(UserWidget))
		{
			Widget->SetOwner(this);
		}
	}
	
}

int32 ADoor::GetDoorNum()
{
	return DoorNum;
}

