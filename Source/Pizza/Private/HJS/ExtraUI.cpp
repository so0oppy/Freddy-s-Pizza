// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/ExtraUI.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include "Runtime/MediaAssets/Public/MediaSource.h"
#include "MediaSoundComponent.h"
#include "HJS/MovieAudio.h"
#include "kismet/GameplayStatics.h"
#include "HJS/bCheatSaveGame.h"
#include "HJS/NightmareMode.h"
void UExtraUI::NativeConstruct()
{
	Super::NativeConstruct();
	// 버튼 바인딩 시키기
	AnimatronicsBtn->OnClicked.AddDynamic(this,&UExtraUI::OnAnimatronics);
	JumpScaresBtn->OnClicked.AddDynamic(this , &UExtraUI::OnJumpScares);
	LeftBtn->OnClicked.AddDynamic(this , &UExtraUI::OnLeft);
	RightBtn->OnClicked.AddDynamic(this , &UExtraUI::OnRight);
	NightmareBtn->OnClicked.AddDynamic(this , &UExtraUI::OnNightmare);
	NightmareStartBtn->OnClicked.AddDynamic(this , &UExtraUI::OnStart);
	CheatsBtn->OnClicked.AddDynamic(this , &UExtraUI::OnCheats);
	HouseMapBtn->OnClicked.AddDynamic(this , &UExtraUI::OnHouseMap);
	FastNightBtn->OnClicked.AddDynamic(this , &UExtraUI::OnFastNights);
	// 초기 화면은 왼쪽 글자들 중 Animatronics가 선택 되어있고, NightmareFreddy가 보이는 상태

	// 텍스쳐 초기화 4개.
	AnimatronicsTextures.Add(FreddyTexture);
	AnimatronicsTextures.Add(BonnieTexture);
	AnimatronicsTextures.Add(ChicaTexture);
	AnimatronicsTextures.Add(FoxyTexture);

	// 미디어 초기화 4개
	MediaSources.Add(MediaSourceFreddy);
	MediaSources.Add(MediaSourceBonnie);
	MediaSources.Add(MediaSourceChica);
	MediaSources.Add(MediaSourceFoxy);

	if ( AnimatronicsTextures.Num() > 0)
	{
		// 첫 이미지는 프레디
		ChangeImg(AnimatronicsNum);
	}

	// 세이브 게임 불러와서 체크 보일지 말지 결정
	if ( UGameplayStatics::DoesSaveGameExist(MySlotName , UserIndex) )
	{
		SaveGameIns = Cast<UbCheatSaveGame>(UGameplayStatics::LoadGameFromSlot(MySlotName , UserIndex));
		if ( !SaveGameIns->bHouseMap )
		{
			HouseMapCheck->SetVisibility(ESlateVisibility::Hidden);
		}
		if ( !SaveGameIns->bFastNight )
		{
			FastNightsCheck->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		HouseMapCheck->SetVisibility(ESlateVisibility::Hidden);
		FastNightsCheck->SetVisibility(ESlateVisibility::Hidden);
	}

	// 처음 화면 구성 (Animatronics가 선택된 상태)
	StateOff(UIState::Cheats);
	StateOff(UIState::Jumpscares);
	StateOff(UIState::Nightmare);
	StateOn(UIState::Animatronics);
}

void UExtraUI::OnAnimatronics()
{
	//	현재 이 상태면 return
	if ( CurrentState == UIState::Animatronics )
	{
		return;
	}
	if ( AudioComp )
	{
		AudioComp->Stop();
	}
	SetState(UIState::Animatronics);
}

void UExtraUI::OnJumpScares()
{
	if ( CurrentState == UIState::Jumpscares )
	{
		return;
	}

	SetState(UIState::Jumpscares);
	if ( AudioComp ) 
	{
		AudioComp->Stop();
	}
	PlayVideo(JumpscaresNum);
}

void UExtraUI::OnLeft()
{
	// 애니매트로닉스일 때
	if ( CurrentState == UIState::Animatronics )
	{
		// 3일때 오른쪽 버튼 켜주기
		if ( AnimatronicsNum == 3 )
		{
			RightBtn->SetVisibility(ESlateVisibility::Visible);
		}

		// 1일 때 왼쪽 버튼 꺼주기
		if ( AnimatronicsNum == 1 )
		{
			LeftBtn->SetVisibility(ESlateVisibility::Hidden);
		}
		// Num을 하나씩 빼주고
		AnimatronicsNum--;
		// 방어코드
		if ( AnimatronicsNum < 0 )
		{
			return;
		}
		ChangeImg(AnimatronicsNum);
	}
	// 점프스케어일 때
	else if ( CurrentState == UIState::Jumpscares )
	{
		// 3일때 오른쪽 버튼 켜주기
		if ( JumpscaresNum == 3 )
		{
			RightBtn->SetVisibility(ESlateVisibility::Visible);
		}

		// 1일 때 왼쪽 버튼 꺼주기
		if ( JumpscaresNum == 1 )
		{
			LeftBtn->SetVisibility(ESlateVisibility::Hidden);
		}
		JumpscaresNum--;
		if ( JumpscaresNum < 0 )
		{
			return;
		}
		AudioComp->Stop();
		PlayVideo(JumpscaresNum);
	}

}

void UExtraUI::OnRight()
{
	// 애니매트로닉스일 때
	if ( CurrentState == UIState::Animatronics )
	{
		// 0일때 왼쪽 버튼 켜주기
		if ( AnimatronicsNum == 0 )
		{
			LeftBtn->SetVisibility(ESlateVisibility::Visible);
		}

		// 2일 때 오른쪽 버튼 꺼주기
		if ( AnimatronicsNum == 2 )
		{
			RightBtn->SetVisibility(ESlateVisibility::Hidden);
		}

		// 번호를 1 더해주고
		AnimatronicsNum++;
		// 방어코드
		if ( AnimatronicsNum >= AnimatronicsTextures.Num() )
		{
			return;
		}
		ChangeImg(AnimatronicsNum);
	}
	// 점프스케어일 때
	else if ( CurrentState == UIState::Jumpscares )
	{

		// 0일때 왼쪽 버튼 켜주기
		if ( JumpscaresNum == 0 )
		{
			LeftBtn->SetVisibility(ESlateVisibility::Visible);
		}

		// 2일 때 오른쪽 버튼 꺼주기
		if ( JumpscaresNum == 2 )
		{
			RightBtn->SetVisibility(ESlateVisibility::Hidden);
		}

		JumpscaresNum++;
		if ( JumpscaresNum >= MediaSources.Num() )
		{
			return;
		}
		AudioComp->Stop();
		PlayVideo(JumpscaresNum);
	}
}

void UExtraUI::OnNightmare()
{
	if ( CurrentState == UIState::Nightmare )
	{
		return;
	}
	SetState(UIState::Nightmare);
	if ( AudioComp )
	{
		AudioComp->Stop();
	}
}

void UExtraUI::OnStart()
{
	// 
	UNightmareMode* NightmareMode = GetGameInstance()->GetSubsystem<UNightmareMode>();
	NightmareMode->bNightmare = true;
	UGameplayStatics::OpenLevel(this , FName("HJSBetaMap"));
}

void UExtraUI::OnCheats()
{
	if ( CurrentState == UIState::Cheats )
	{
		return;
	}

	SetState(UIState::Cheats);
	if ( AudioComp )
	{
		AudioComp->Stop();
	}
}

void UExtraUI::OnHouseMap()
{
	if ( UGameplayStatics::DoesSaveGameExist(MySlotName , UserIndex) )
	{
		SaveGameIns = Cast<UbCheatSaveGame>(UGameplayStatics::LoadGameFromSlot(MySlotName , UserIndex));

		if ( SaveGameIns->bHouseMap == true )
		{
			SaveHouseMap(false);
			HouseMapCheck->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			SaveHouseMap(true);
			HouseMapCheck->SetVisibility(ESlateVisibility::Visible);
		}
	}
	// 아무것도 없으면 false 상태이니 true로 저장
	else
	{
		SaveHouseMap(true);
		HouseMapCheck->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void UExtraUI::OnFastNights()
{
	if ( UGameplayStatics::DoesSaveGameExist(MySlotName , UserIndex) )
	{
		SaveGameIns = Cast<UbCheatSaveGame>(UGameplayStatics::LoadGameFromSlot(MySlotName , UserIndex));

		if ( SaveGameIns->bFastNight == true )
		{
			SaveFastNight(false);
			FastNightsCheck->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			SaveFastNight(true);
			FastNightsCheck->SetVisibility(ESlateVisibility::Visible);
		}
	}
	// 아무것도 없으면 false 상태이니 true로 저장
	else
	{
		SaveFastNight(true);
		FastNightsCheck->SetVisibility(ESlateVisibility::Visible);
	}

}

void UExtraUI::ChangeImg(int32 Num)
{
	if ( AnimatronicsImg )
	{
		// 이미지 변경해주기
		AnimatronicsImg->SetBrushFromTexture(AnimatronicsTextures[Num]);
	}

	AnimatronicsText->SetText(FText::FromString(AnimatronicsStrings[Num]));

}

void UExtraUI::StateOn(UIState State)
{
	
	// 인자값으로 들어온 State에서 필요한 요소들을 켜주는 함수
	switch ( State )
	{
	// 애니매 버튼 눌렀을 때 켜줘야 할 것 (텍스트, 중앙 이미지, 밑줄, 오른쪽 버튼)
	case UIState::Animatronics:
		AnimatronicsText->SetVisibility(ESlateVisibility::Visible);
		AnimatronicsImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		AnimatronicsBorder->SetVisibility(ESlateVisibility::Visible);
		RightBtn->SetVisibility(ESlateVisibility::Visible);
		break;
	// 점프스케어 버튼 눌렀을 때 켜줘야 할 것 (중앙 영상, 밑줄, 오른쪽 버튼)
	case UIState::Jumpscares:
		JumpScaresBorder->SetVisibility(ESlateVisibility::Visible);
		RightBtn->SetVisibility(ESlateVisibility::Visible);
		JumpscareMedia->SetVisibility(ESlateVisibility::Visible);
		break;
	// 나이트메어 버튼 눌렀을 때 켜줘야 할 것 (밑줄, 스타트 버튼)
	case UIState::Nightmare:
		NightmareStartBtn->SetVisibility(ESlateVisibility::Visible);
		NightmareBorder->SetVisibility(ESlateVisibility::Visible);
		break;
	// 치트 버튼 눌렀을 때 켜줘야 할 것 (밑줄, 치트서브(하우스맵 버튼, 패스트나이트 버튼))
	case UIState::Cheats:
		CheatsBorder->SetVisibility(ESlateVisibility::Visible);
		CheatSub->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		//HouseMapBtn->SetVisibility(ESlateVisibility::Visible);
		//FastNightBtn->SetVisibility(ESlateVisibility::Visible);
		break;
	}

}

void UExtraUI::StateOff(UIState State)
{
	// 인자값으로 들어온 State에서 필요한 요소들을 꺼주는 함수
	switch ( State )
	{
		// 애니매 버튼 눌렀을 때 꺼줘야 할 것 (텍스트, 중앙 이미지, 밑줄, 오른쪽 버튼, 왼쪽 버튼)
	case UIState::Animatronics:
		AnimatronicsText->SetVisibility(ESlateVisibility::Hidden);
		AnimatronicsImg->SetVisibility(ESlateVisibility::Hidden);
		AnimatronicsBorder->SetVisibility(ESlateVisibility::Hidden);
		RightBtn->SetVisibility(ESlateVisibility::Hidden);
		LeftBtn->SetVisibility(ESlateVisibility::Hidden);
		AnimatronicsNum = 0;
		ChangeImg(AnimatronicsNum);
		break;
		// 점프스케어 버튼 눌렀을 때 켜줘야 할 것 (중앙 영상, 밑줄, 오른쪽 버튼, 왼쪽 버튼)
	case UIState::Jumpscares:
		JumpScaresBorder->SetVisibility(ESlateVisibility::Hidden);
		RightBtn->SetVisibility(ESlateVisibility::Hidden);
		LeftBtn->SetVisibility(ESlateVisibility::Hidden);
		JumpscareMedia->SetVisibility(ESlateVisibility::Hidden);
		JumpscaresNum = 0;
		break;
		// 나이트메어 버튼 눌렀을 때 켜줘야 할 것 (밑줄, 스타트 버튼)
	case UIState::Nightmare:
		NightmareStartBtn->SetVisibility(ESlateVisibility::Hidden);
		NightmareBorder->SetVisibility(ESlateVisibility::Hidden);
		break;
		// 치트 버튼 눌렀을 때 켜줘야 할 것 (밑줄, 치트서브(하우스맵 버튼, 패스트나이트 버튼))
	case UIState::Cheats:
		CheatsBorder->SetVisibility(ESlateVisibility::Hidden);
		CheatSub->SetVisibility(ESlateVisibility::Hidden);
		//HouseMapBtn->SetVisibility(ESlateVisibility::Visible);
		//FastNightBtn->SetVisibility(ESlateVisibility::Visible);
		break;
	}
}

void UExtraUI::SetState(UIState NextState)
{
	// 현재 CurrentState의 기능을 모두 꺼주고
	StateOff(CurrentState);
	// CurrentState를 NextState로 변경
	CurrentState = NextState;
	// 변경된 CurrentState의 기능을 켜주기
	StateOn(CurrentState);
}

void UExtraUI::PlayVideo(int32 Num)
{
	// MediaSource를 설정하고, MediaPlayer를 통해 동영상 재생
	if(MediaPlayer && MediaSources[Num] ) {
		MediaPlayer->OpenSource(MediaSources[Num]);
		MediaPlayer->Rewind();
		MediaPlayer->Play();
		AudioComp = UGameplayStatics::SpawnSound2D(GetWorld(),Audio);
		AudioComp->Play();
	}
}

void UExtraUI::SaveHouseMap(bool Value)
{
	if ( SaveGameIns )
	{
		SaveGameIns->bHouseMap = Value;
		UGameplayStatics::SaveGameToSlot(SaveGameIns , MySlotName , UserIndex);
	}
	else
	{
		if ( UGameplayStatics::DoesSaveGameExist(MySlotName , UserIndex) )
		{
			SaveGameIns = Cast<UbCheatSaveGame>(UGameplayStatics::LoadGameFromSlot(MySlotName , UserIndex));
		}
		else
		{
			SaveGameIns = Cast<UbCheatSaveGame>(UGameplayStatics::CreateSaveGameObject(UbCheatSaveGame::StaticClass()));
		}
		if(SaveGameIns )
		{
			SaveGameIns->bHouseMap = Value;
			UGameplayStatics::SaveGameToSlot(SaveGameIns , MySlotName , UserIndex);
		}
	}

}

void UExtraUI::SaveFastNight(bool Value)
{
	if ( SaveGameIns )
	{
		SaveGameIns->bFastNight = Value;
		UGameplayStatics::SaveGameToSlot(SaveGameIns , MySlotName , UserIndex);
	}
	else
	{
		if ( UGameplayStatics::DoesSaveGameExist(MySlotName , UserIndex) )
		{
			SaveGameIns = Cast<UbCheatSaveGame>(UGameplayStatics::LoadGameFromSlot(MySlotName , UserIndex));
		}
		else
		{
			SaveGameIns = Cast<UbCheatSaveGame>(UGameplayStatics::CreateSaveGameObject(UbCheatSaveGame::StaticClass()));
		}
		if ( SaveGameIns )
		{
			SaveGameIns->bFastNight = Value;
			UGameplayStatics::SaveGameToSlot(SaveGameIns , MySlotName , UserIndex);
		}
	}
}

