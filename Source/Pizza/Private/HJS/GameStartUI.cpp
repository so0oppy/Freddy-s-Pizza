// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/GameStartUI.h"
#include "Components/Image.h"
#include "HJS/HJSGameMode.h"
#include "HJS/TimeUI.h"

void UGameStartUI::NativeTick(const FGeometry& MyGeometry , float InDeltaTime)
{
	Super::NativeTick(MyGeometry , InDeltaTime);

	if ( bStart )
	{
		CurrentTime += InDeltaTime;

		if ( CurrentTime >= StartTime )
		{
			Resume();
			bStart = false;
		}
	}
}

void UGameStartUI::PlayFadeInAnim()
{
	PlayAnimation(FadeIn);
}

void UGameStartUI::HideImage()
{
	Start->SetVisibility(ESlateVisibility::Hidden);
}

void UGameStartUI::GameStart(UTimeUI* TUI)
{
	PlayAnimation(FadeIn);
	bStart = true;
	if ( TUI )
	{
		TimeUI = TUI;
	}
}

void UGameStartUI::Resume()
{
	PlayFadeInAnim();
	HideImage();
	AHJSGameMode* GM = Cast<AHJSGameMode>(GetWorld()->GetAuthGameMode());
	GM->GameStartAnim();
	if ( TimeUI )
	{
		TimeUI->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}
