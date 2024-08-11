// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()
	

public:
	
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UTexture2D*> ImageSequence;

	UPROPERTY(EditDefaultsOnly)
	int32 CurrentFrameIndex;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite)
    float FrameRate;

private:
	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* FadeIn;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UImage* MainImg;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* Start;

	FTimerHandle FrameTimerHandle;

	void UpdateFrame();

	UFUNCTION()
    void OnStartButtonClicked();

};
