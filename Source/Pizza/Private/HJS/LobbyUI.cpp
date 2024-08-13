// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/LobbyUI.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
void ULobbyUI::NativeConstruct()
{
	// 현재 프레임 인덱스 초기화
	CurrentFrameIndex = 0;

    // 텍스처 경로와 패턴
    FString TexturePath = "'/Game/HJS/Assets/LobbyFrame/";

    // 100개의 텍스처 로드
    for ( int32 i = 1; i <= 62; ++i )
    {
        FString TextureNumber = FString::Printf(TEXT("Lobby-%03d.Lobby-%03d'") , i,i); // 001, 002, ..., 100
        FString FullPath = TexturePath + TextureNumber;

        // 경로에서 텍스처 로드
        UTexture2D* Texture = LoadObject<UTexture2D>(nullptr , *FullPath);
        if ( Texture )
        {
            ImageSequence.Add(Texture);
        }
    }

	// 타이머 시작, FrameRate는 초당 프레임 수입니다.
	GetWorld()->GetTimerManager().SetTimer(FrameTimerHandle , this , &ULobbyUI::UpdateFrame , FrameRate , true);

    if ( Start )
    {
        Start->OnClicked.AddDynamic(this , &ULobbyUI::OnStartButtonClicked);
    }

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
    UGameplayStatics::OpenLevel(this , FName("HJSAlphaMap"));
}
