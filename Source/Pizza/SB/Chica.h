#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SB/LocationState.h" // E열거형 공유 파일
#include "Chica.generated.h"
#include "LocationState.h"

UCLASS()
class PIZZA_API AChica : public ACharacter
{
	GENERATED_BODY()

public:
	AChica();

	void SetUpLocation(ELocationState State);

protected:
	virtual void BeginPlay() override;
	

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	ELocationState CurrentState;
};
