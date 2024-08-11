// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStartUI.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UGameStartUI : public UUserWidget
{
	GENERATED_BODY()
	


public:
	
	virtual void NativeTick(const FGeometry& MyGeometry , float InDeltaTime) override;

	void PlayFadeInAnim();

	void HideImage();

	void GameStart(class UTimeUI* TUI);

	void Resume();

	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* FadeIn;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UImage* Start;

	UPROPERTY(EditAnywhere)
	float StartTime = 7.f;
	float CurrentTime = 0.f;
	bool bStart = false;

	UPROPERTY()
	class UTimeUI* TimeUI;

};
