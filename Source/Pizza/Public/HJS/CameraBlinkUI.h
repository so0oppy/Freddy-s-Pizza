// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CameraBlinkUI.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UCameraBlinkUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void PlayBlinkAnim();

private:
	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* CameraBlink_1;
	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* CameraBlink_2;
	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* CameraBlink_3;

};
