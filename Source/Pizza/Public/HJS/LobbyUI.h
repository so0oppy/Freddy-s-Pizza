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
    float FrameRate = 2.f;

private:
	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* FadeIn;

	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* FadeOut;

	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* ExtraFadeIn;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UImage* MainImg;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* Start;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* Extra;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UExtraUI* ExtraUI;

	UPROPERTY(meta=(BindWidget))
	UButton* ExitBtn;

	FTimerHandle FrameTimerHandle;

	void UpdateFrame();

	UFUNCTION()
    void OnStartButtonClicked();

	UFUNCTION()
	void OnExtra();

	UFUNCTION()
	void ExtraUISet();
	UFUNCTION()
	void MainUISet();

	// Exit
	UFUNCTION()
	void OnExit();

};
