// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/HJSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SB/Chica.h"
#include "SB/AILevel.h"
#include "SB/Foxy.h"
#include "JYS/EnemyBonnie.h"
#include "JYS/EnemyFreddy.h"
#include "HJS/MinimapUI.h"
#include "HJS/GameStartUI.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/AudioComponent.h"
#include "HJS/ClearGameUI.h"
#include "HJS/TimeUI.h"

AHJSGameMode::AHJSGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHJSGameMode::BeginPlay()
{

	Super::BeginPlay();
	Chica = Cast<AChica>(UGameplayStatics::GetActorOfClass(GetWorld() , AChica::StaticClass()));
	check(Chica);
	Bonnie = Cast<AEnemyBonnie>(UGameplayStatics::GetActorOfClass(GetWorld() , AEnemyBonnie::StaticClass()));
	check(Bonnie);
	Foxy = Cast<AFoxy>(UGameplayStatics::GetActorOfClass(GetWorld() , AFoxy::StaticClass()));
	check(Foxy);
	Freddy = Cast<AEnemyFreddy>(UGameplayStatics::GetActorOfClass(GetWorld() , AEnemyFreddy::StaticClass()));
	check(Freddy);
	LV_freddy = { {0,0,1,2,2,2}, {2,2,2,3,3,3}, {3,3,3,3,3,3}, {4,4,4,4,4,4} };
	LV_bonnie = { {0,0,1,3,3,3}, {5,5,5,7,7,7}, {7,7,7,10,10,10}, {10,10,10,12,12,12} };
	//Enemy 초기 레벨 세팅 여기서 해주기
	SetAILevelEnemy();
	if ( GEngine )
	{
		//GEngine->AddOnScreenDebugMessage(-1 , 30.f , FColor::Red , TEXT("12 AM"));
	}
	GetWorldTimerManager().SetTimer(HourHandle,this, &AHJSGameMode::TimePass,GamePlayRate,true);

	// 미니맵 띄우기


	// 게임을 시작하면 일단 퍼즈를 걸기.
	UGameplayStatics::SetGamePaused(GetWorld() , true);
	// 퍼즈를 건 채로 UI 애니메이션을 재생하고
	if ( TimeUIFactory )
	{
		// 뷰포트에 띄우기
		TimeUI = Cast<UTimeUI>(CreateWidget(GetWorld() , TimeUIFactory , FName("TimeUI")));
		if ( TimeUI )
		{
			TimeUI->AddToViewport();
			TimeUI->SetTime(TEXT("12AM"));
			TimeUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if ( GameStartUIFactory )
	{
		// 뷰포트에 띄우기
		GameStartUI = Cast<UGameStartUI>(CreateWidget(GetWorld() , GameStartUIFactory , FName("GameStartUI")));
		if ( GameStartUI )
		{
			GameStartUI->AddToViewport();
			GameStartUI->GameStart(TimeUI);
		}
	}

	// 배경음악 재생
	if ( BGM )
	{
		BGMComponent = UGameplayStatics::SpawnSound2D(GetWorld() , BGM);
		CricketsComponent= UGameplayStatics::SpawnSound2D(GetWorld() , Crickets);
	}

}	



void AHJSGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if ( MinimapUI )
	{
		
		// 윤선
		MinimapUI->UpdateRoom(static_cast<int32>(Bonnie->State), 0, 0);

		// 수빈
		MinimapUI->UpdateRoom(Chica->RoomNum , 1 , 1);
		if ( Foxy ) {
			MinimapUI->UpdateRoom(Foxy->RoomNum , 2 , 1);
		}
	}


}

AChica* AHJSGameMode::GetChica()
{
	return Chica;
}

AEnemyBonnie* AHJSGameMode::GetBonnie()
{
	return Bonnie;
}

void AHJSGameMode::TimePass()
{
	// 시간이 지나고, Enemy들을 업데이트 시켜주기.
	Hour++;
	FString String = FString::Printf(TEXT("%dAM") , (int32)Hour);

	TimeUI->SetTime(String);

	if ( Hour >= 6 )
	{
		DayPass();
	}
	else {
		SetAILevelEnemy();
	}
}

void AHJSGameMode::MiniMapAdd()
{
if ( MinimapUIFactory )
{
	// 뷰포트에 띄우기
	MinimapUI = Cast<UMinimapUI>(CreateWidget(GetWorld() , MinimapUIFactory , FName("MiniMapUI")));

	if ( MinimapUI )
	{
		MinimapUI->AddToViewport();
	}

}
}

void AHJSGameMode::DayPass()
{
	// 날짜가 지나면 시연때는 클리어, 본 게임에서는 다음날로 넘어가기
	Day++;
	Hour = 0;
	GameEnd();
}

void AHJSGameMode::SetAILevelEnemy()
{
	// 수빈Enemy는 AILevel 클래스의 날짜와 시간을 업데이트 해주면 된다.	
	if ( Chica )
	{
		UAILevel* ChicaAILevel = Chica->AILevelComp;
		if ( ChicaAILevel )
		{
			ChicaAILevel->Day = Day;
			ChicaAILevel->Hour = Hour;
		}
	}
	if ( Foxy )
	{
		UAILevel* FoxyAILevel = Foxy->AILevelComp;
		if ( FoxyAILevel )
		{
			FoxyAILevel->Day = Day;
			FoxyAILevel->Hour = Hour;
		}
	}
	// 윤선Enemy는 SetAILevel함수로 값을 설정해 준다.
	if ( Bonnie )
	{
		Bonnie->SetAILevel(LV_bonnie[Day][Hour]);
		//Bonnie->SetAILevel(20);
	}

	if ( Freddy )
	{
		Freddy->SetAILevel(LV_freddy[Day][Hour]);
		//Freddy->SetAILevel(20);
	}

}

void AHJSGameMode::GameEnd()
{
	if ( BGMComponent )
	{
		BGMComponent->Stop();
	}

	if ( CricketsComponent )
	{
		CricketsComponent->Stop();
	}

	UGameplayStatics::SpawnSound2D(GetWorld() , Alarm);

	if ( ClearGameUIFactory )
	{
		// 뷰포트에 띄우기
		ClearGameUI = Cast<UClearGameUI>(CreateWidget(GetWorld() , ClearGameUIFactory , FName("ClearGameUI")));
		if ( ClearGameUI )
		{
			ClearGameUI->AddToViewport();
			ClearGameUI->PlayBlink();
		}
	}
	UGameplayStatics::SetGamePaused(GetWorld() , true);

}

void AHJSGameMode::GameStartAnim()
{
	// 퍼즈를 풀기
	UGameplayStatics::SetGamePaused(GetWorld() , false);

}

