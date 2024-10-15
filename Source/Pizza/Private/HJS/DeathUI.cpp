// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/DeathUI.h"
#include "HJS/HJSGameMode.h"
#include "Kismet/GameplayStatics.h"
void UDeathUI::NativeTick(const FGeometry& MyGeometry , float InDeltaTime)
{
	Super::NativeTick(MyGeometry , InDeltaTime);

	if ( bStart )
	{
		CurrentTime += InDeltaTime;

		if ( CurrentTime >= StartTime )
		{
			Restart();
			bStart = false;
		}
	}
}

void UDeathUI::GameEnd()
{
	bStart = true;
	PlayAnimation(FadeOut);

}

void UDeathUI::Restart()
{
	AHJSGameMode* GM = Cast<AHJSGameMode>(GetWorld()->GetAuthGameMode());
	GM->GameStartAnim();

	UGameplayStatics::OpenLevel(this , FName("LobbyLevel"));

}