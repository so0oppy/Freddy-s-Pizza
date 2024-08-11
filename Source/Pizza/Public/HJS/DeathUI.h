// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathUI.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UDeathUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry , float InDeltaTime) override;

	void GameEnd();

	void Restart();

private:
	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* FadeOut;

	UPROPERTY(EditAnywhere)
	float StartTime = 5.f;
	float CurrentTime = 0.f;
	bool bStart = false;
};
