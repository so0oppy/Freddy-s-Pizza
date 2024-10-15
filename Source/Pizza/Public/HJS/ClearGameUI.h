// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClearGameUI.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UClearGameUI : public UUserWidget
{
	GENERATED_BODY()



public:

	virtual void NativeTick(const FGeometry& MyGeometry , float InDeltaTime) override;

	UFUNCTION()
	void PlayBlink();

	void Restart();
private:
	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* Blink;

	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim),Transient)
	UWidgetAnimation* Why;

	UPROPERTY(EditAnywhere)
	float EndTime = 6.5f;
	float CurrentTime = 0.f;
	bool bStart = false;


};
