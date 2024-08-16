// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/LobbyUI.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "HJS/ExtraUI.h"
#include "HJS/NightmareMode.h"
void ULobbyUI::NativeConstruct()
{
    Super::NativeConstruct();
	// 현재 프레임 인덱스 초기화
	CurrentFrameIndex = 0;

    // 텍스처 경로와 패턴
    FString TexturePath = "'/Game/HJS/Assets/LobbyFrame/";

    // 100개의 텍스처 로드
    //for ( int32 i = 1; i <= 62; ++i )
    //{
    //    FString TextureNumber = FString::Printf(TEXT("Lobby-%03d.Lobby-%03d'") , i,i); // 001, 002, ..., 100
    //    FString FullPath = TexturePath + TextureNumber;

    //    // 경로에서 텍스처 로드
    //    UTexture2D* Texture = LoadObject<UTexture2D>(nullptr , *FullPath);
    //    if ( Texture )
    //    {
    //        ImageSequence.Add(Texture);
    //    }
    //}

	// 타이머 시작, FrameRate는 초당 프레임 수입니다.
	//

    if ( Start )
    {
        Start->OnClicked.AddDynamic(this , &ULobbyUI::OnStartButtonClicked);
    }

    if ( Extra )
    {
        Extra->OnClicked.AddDynamic(this, &ULobbyUI::OnExtra);
    }
    if ( ExitBtn )
    {
        ExitBtn->OnClicked.AddDynamic(this , &ULobbyUI::OnExit);
    }
    ExtraUI->SetVisibility(ESlateVisibility::Hidden);
    ExitBtn->SetVisibility(ESlateVisibility::Hidden);
    PlayAnimation(FadeIn);
}

void ULobbyUI::UpdateFrame()
{
	if ( ImageSequence.Num() > 0 && MainImg )
	{
		// 현재 프레임을 설정
		MainImg->SetBrushFromTexture(ImageSequence[CurrentFrameIndex]);

		// 다음 프레임으로 인덱스 증가
		CurrentFrameIndex = (CurrentFrameIndex + 1) % ImageSequence.Num();
	}
}

void ULobbyUI::OnStartButtonClicked()
{
    UNightmareMode* NightmareMode = GetGameInstance()->GetSubsystem<UNightmareMode>();
    NightmareMode->bNightmare = false;
    UGameplayStatics::OpenLevel(this , FName("HJSBetaMap"));
}

void ULobbyUI::OnExtra()
{
    // FadeOut하기
    PlayAnimation(FadeOut);
    ExtraUI->SetColorAndOpacity(FLinearColor(1.f,1.f,1.f,1.f));
    // FadeOut이 끝나는 시점에 UI 띄우기
    // UI가 띄우는 시점에 FadeIn하기
    GetWorld()->GetTimerManager().SetTimer(FrameTimerHandle , this , &ULobbyUI::ExtraUISet , FrameRate , false);
}

void ULobbyUI::ExtraUISet()
{  
   // UI 띄우기
   ExtraUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
   ExitBtn->SetVisibility(ESlateVisibility::Visible);
   // Fade In
   PlayAnimation(FadeIn);
}

void ULobbyUI::MainUISet()
{
    ExtraUI->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyUI::OnExit()
{
    ExtraUI->SetVisibility(ESlateVisibility::HitTestInvisible);
    PlayAnimation(ExtraFadeIn);
    GetWorld()->GetTimerManager().SetTimer(FrameTimerHandle , this , &ULobbyUI::MainUISet , FrameRate , false);
    ExitBtn->SetVisibility(ESlateVisibility::Hidden);
}


