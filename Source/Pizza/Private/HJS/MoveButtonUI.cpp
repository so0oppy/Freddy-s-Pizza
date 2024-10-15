// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/MoveButtonUI.h"
#include "Components/Button.h"
#include "HJS/Door.h"
#include "HJS/FreddyPlayer.h"

void UMoveButtonUI::SetOwner(AActor* NewActor)
{
	ADoor* TempDoor = Cast<ADoor>(NewActor);
	if(TempDoor)
	{
		Owner = TempDoor;
	}
}

void UMoveButtonUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (!Door)
	{
		UE_LOG(LogTemp, Error, TEXT("SomeObject is null!"));
		return;
	}
	Door->OnClicked.AddDynamic(this, &UMoveButtonUI::Move);
}

void UMoveButtonUI::Move()
{
	if (Owner)
	{
		int32 DoorNum = Owner->GetDoorNum();
		//UE_LOG(LogTemp,Warning,TEXT("%d"), DoorNum);
		AFreddyPlayer* Player = Cast<AFreddyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Player)
		{
			Player->SetMoveDoor(DoorNum);
		}
		
	}
}
