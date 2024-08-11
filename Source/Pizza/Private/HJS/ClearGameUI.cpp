// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/ClearGameUI.h"
#include "HJS/HJSGameMode.h"
#include "Kismet/GameplayStatics.h"
void UClearGameUI::NativeTick(const FGeometry& MyGeometry , float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

	if ( bStart )
	{
		CurrentTime += InDeltaTime;

		if ( CurrentTime >= EndTime )
		{
			Restart();
			bStart = false;
		}
	}
}

void UClearGameUI::PlayBlink()
{
	PlayAnimation(Blink);
	//PlayAnimation(Why);
	bStart = true;
}


void UClearGameUI::Restart()
{
	AHJSGameMode* GM = Cast<AHJSGameMode>(GetWorld()->GetAuthGameMode());
	GM->GameStartAnim();
	UGameplayStatics::OpenLevel(this , FName("LobbyLevel"));
}