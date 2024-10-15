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

	int32 Day = 3; // 3����, (1~4�������� +1 ����, ���� ���Ӹ�忡�� �ҷ�������)
	int32 Hour = 0; // 12AM , 1�ð��� 5AM���� +1�� ����, ���� ���Ӹ�忡�� �ҷ�������
	int32 Level = 1; // ������ 0~20, �迭�� �����ΰ� �ð� ���� ������ �ڵ� �����ǰ�

	// ���� = 1 �� ���� �Ĵٺ��⸸ ��, 2 �̻���� �� ������ ��

	TArray<TArray<int32>> LV_freddy;
	TArray<TArray<int32>> LV_bonny;
	TArray<TArray<int32>> LV_chica;
	TArray<TArray<int32>> LV_foxy;

	UPROPERTY()
	class AChica* Chica;
	class AFoxy* Foxy;
	// ������, ���� �߰�

	void SetLevel(ACharacter* character);
	bool RandomMove(ACharacter* character, float DeltaTime);
};
