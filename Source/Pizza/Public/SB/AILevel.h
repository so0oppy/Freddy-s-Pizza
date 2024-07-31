#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AILevel.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PIZZA_API UAILevel : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAILevel();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int32 Day = 3; // 3일차, (1~4일차까지 +1 증가, 추후 게임모드에서 불러오도록)
	int32 Hour = 0; // 12AM , 1시간씩 5AM까지 +1씩 증가, 추후 게임모드에서 불러오도록
	int32 Level = 0; // 레벨은 0~20, 배열로 만들어두고 시간 지날 때마다 자동 설정되게

	// 레벨 = 1 일 때는 쳐다보기만 함, 2 이상부터 문 앞으로 옴

	TArray<TArray<int32>> LV_freddy;
	TArray<TArray<int32>> LV_bonny;
	TArray<TArray<int32>> LV_chica;
	TArray<TArray<int32>> LV_foxy;

	UPROPERTY()
	class AChica* Chica;
	//class AFoxy* Foxy;
	// 프레디, 보니 추가

	void SetLevel(ACharacter* character);
	void RandomMove(ACharacter* character, float DeltaTime, FName Tag);
};
