#include "SB/AILevel.h"
#include "SB/LocationState.h"
#include "SB/AllEnemy.h"
#include "SB/Chica.h"


// Sets default values for this component's properties
UAILevel::UAILevel()
{
	PrimaryComponentTick.bCanEverTick = true;

	LV_chica = {{0,0,1,2,2,2}, {5,5,5,7,7,7}, {7,7,7,10,10,10}, {10,10,10,12,12,12}};
	LV_foxy = { {0,0,0,0,0,0}, {1,1,1,4,4,4}, {10,10,10,10,10,10}, {5,5,5,10,10,10} };
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


void UAILevel::SetLevel(ACharacter* character)
{
	//Level = Cast<AAllEnemy>(character)->Level;
	TArray<int32> tempLV;

	Chica = Cast<AChica>(character);

	// 시간마다 레벨 다르게 설정
	if (character == Chica) // 각 캐릭터 코드에서 이 함수 불러오기 위함
	{
		tempLV = LV_chica[Day - 1]; // 각 Day 내의 정보를 저장
	}
	//else if(character == Foxy)
	
	Level = tempLV[Hour]; // 그 정보 내의 시간별로 레벨 할당
}

void UAILevel::RandomMove(ACharacter* character,float DeltaTime)
{
	Chica = Cast<AChica>(character);

	// 1~20 사이 랜덤 숫자 <= AI 레벨일 경우 이동 (확률로)
	int32 rand = FMath::RandRange(1, 20);
	int32 moveRand = FMath::RandRange(0, 99);
	if (rand <= Level)
	{
		if (moveRand < 5 * Level) // 레벨마다 확률 낮아지므로
		{
			// 다른 방으로 이동
			if(Chica)
				Chica->SetUpLocation(ELocationState::MOVE, DeltaTime);
			// else if(character == Foxy)
		}
	}
}