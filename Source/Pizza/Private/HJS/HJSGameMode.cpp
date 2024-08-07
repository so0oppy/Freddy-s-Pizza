// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/HJSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SB/Chica.h"
#include "SB/AILevel.h"
#include "SB/Foxy.h"
#include "JYS/EnemyBonnie.h"
#include "JYS/EnemyFreddy.h"
#include "HJS/MinimapUI.h"

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
		GEngine->AddOnScreenDebugMessage(-1 , 30.f , FColor::Red , TEXT("12 AM"));
	}
	GetWorldTimerManager().SetTimer(HourHandle,this, &AHJSGameMode::TimePass,GamePlayRate,true);

	// 미니맵 띄우기
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
	if ( GEngine )
	{
		FString String = FString::Printf(TEXT("%d AM") , (int32)Hour);
		GEngine->AddOnScreenDebugMessage(-1 , 30.f , FColor::Red , String);
	}
	if ( Hour >= 6 )
	{
		DayPass();
	}
	else {
		SetAILevelEnemy();
	}
}

void AHJSGameMode::DayPass()
{
	// 날짜가 지나면 시연때는 클리어, 본 게임에서는 다음날로 넘어가기
	Day++;
	Hour = 0;
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
