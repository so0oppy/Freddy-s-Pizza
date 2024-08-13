// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/DownMouseUI.h"
#include "Components/Button.h"
#include "HJS/FreddyPlayer.h"
#include "Kismet/GamePlayStatics.h"
void UDownMouseUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (!MouseDown)
	{
		UE_LOG(LogTemp, Error, TEXT("SomeObject is null!"));
		return;
	}
	MouseDown->OnHovered.AddDynamic(this, &UDownMouseUI::Down);

	MouseDown->OnUnhovered.AddDynamic(this, &UDownMouseUI::Up);
}

void UDownMouseUI::Down()
{

	if ( UGameplayStatics::IsGamePaused(GetWorld()) )
	{
		return;
	}

	if (Player)
	{
		Player->SetDown();
	}
}

void UDownMouseUI::Up()
{
	if ( UGameplayStatics::IsGamePaused(GetWorld()) )
	{
		return;
	}
	Player->SetUp();
}

void UDownMouseUI::SetPlayer(AFreddyPlayer* NewPlayer)
{
	Player = NewPlayer;
}
