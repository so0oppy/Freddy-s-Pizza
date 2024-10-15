#include "SB/AILevel.h"
#include "SB/LocationState.h"
#include "SB/AllEnemy.h"
#include "SB/Chica.h"
#include "SB/Foxy.h"


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
	Foxy = Cast<AFoxy>(character);

	// �ð����� ���� �ٸ��� ����
	if (nullptr != Chica) // �� ĳ���� �ڵ忡�� �� �Լ� �ҷ����� ����
	{
		tempLV = LV_chica[Day - 1]; // �� Day ���� ������ ����
	}
	else if(nullptr != Foxy)
	{
		tempLV = LV_foxy[Day - 1]; // �� Day ���� ������ ����
	}
	
	//Level = tempLV[Hour]; // �� ���� ���� �ð����� ���� �Ҵ�
	Level = 20;
}

bool UAILevel::RandomMove(ACharacter* character,float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Random Move()"));
	Chica = Cast<AChica>(character);
	Foxy = Cast<AFoxy>(character);

	// 1~20 ���� ���� ���� <= AI ������ ��� �̵� (Ȯ����)
	int32 rand = FMath::RandRange(1, 20);
	int32 moveRand = FMath::RandRange(0, 99);
	if (rand <= Level)
	{
		if (moveRand < 5 * Level) // �������� Ȯ�� �������Ƿ�
		{
			// �ٸ� ������ �̵�
			if(nullptr != Chica)
				return true;
			else if(nullptr != Foxy)
				return true;
		}
		else
		{
			return false;
		}
	}
		
	return false;
}