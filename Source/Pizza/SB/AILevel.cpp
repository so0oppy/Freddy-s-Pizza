#include "AILevel.h"
#include "AILevel.h"
#include "LocationState.h"
#include "AllEnemy.h"


// Sets default values for this component's properties
UAILevel::UAILevel()
{
	PrimaryComponentTick.bCanEverTick = true;

	LV_chica = {(0,0,1,2,2,2), (5,5,5,7,7,7), (7,7,7,10,10,10), (10,10,10,12,12,12)};
	LV_foxy = { (0,0,0,0,0,0), (1,1,1,4,4,4), (10,10,10,10,10,10), (5,5,5,10,10,10) };
}


// Called when the game starts
void UAILevel::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UAILevel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UAILevel::SetLevel()
{
	//Level = Cast<AAllEnemy>(character)->Level;
	TArray<int32> tempLV;

	// 시간마다 레벨 다르게 설정
	tempLV = LV_chica[Day - 1]; // 각 Day 내의 정보를 저장
	Level = tempLV[Hour]; // 그 정보 내의 시간별로 레벨 할당

	// 근데 chica인지 뭔지 구분해야 함........................
}

void UAILevel::RandomMove(ACharacter* character)
{
	// 1~20 사이 랜덤 숫자 <= AI 레벨일 경우 이동 (확률로)
	int32 rand = FMath::RandRange(1, 20);
	int32 moveRand = FMath::RandRange(0, 99);
	if (rand <= Level)
	{
		if (moveRand < 5 * Level) // 레벨마다 확률 낮아지므로
		{
			// 다른 방으로 이동
			Cast<AAllEnemy>(character)->SetUpLocation(ELocationState::Move);
		}
	}
}