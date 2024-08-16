// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExtraUI.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UExtraUI : public UUserWidget
{
	GENERATED_BODY()
	
	enum class UIState
	{
		Animatronics,
		Jumpscares,
		Nightmare,
		Cheats,
	};

public:

	virtual void NativeConstruct() override;
	
	// 순서대로 선언
	UPROPERTY(meta=(BindWidget))
	class UButton* AnimatronicsBtn;
	UPROPERTY(meta=(BindWidget))
	class UBorder* AnimatronicsBorder;
	UPROPERTY(meta=(BindWidget))	
	UButton* JumpScaresBtn;
	UPROPERTY(meta=(BindWidget))
	UBorder* JumpScaresBorder;
	UPROPERTY(meta=(BindWidget))
	UButton* NightmareBtn;
	UPROPERTY(meta=(BindWidget))
	UBorder* NightmareBorder;
	UPROPERTY(meta=(BindWidget))
	UButton* CheatsBtn;
	UPROPERTY(meta=(BindWidget))
	UBorder* CheatsBorder;
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* CheatSub;
	UPROPERTY(meta=(BindWidget))
	UButton* HouseMapBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* FastNightBtn;
	UPROPERTY(meta=(BindWidget))
	class UImage* HouseMapCheck;
	UPROPERTY(meta=(BindWidget))
	UImage* FastNightsCheck;

	UPROPERTY(meta=(BindWidget))
	class UImage* BGImage;

	UPROPERTY(meta=(BindWidget))
	UButton* NightmareStartBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* RightBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* LeftBtn;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* AnimatronicsText;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* FreddyTexture;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* BonnieTexture;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* ChicaTexture;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* FoxyTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UTexture2D*> AnimatronicsTextures;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FString> AnimatronicsStrings = {TEXT("Nightmare Freddy"), TEXT("Nightmare Bonnie"), TEXT("Nightmare Chica"), TEXT("NightMare Foxy")};

	UPROPERTY(meta=(BindWidget))
	UImage* AnimatronicsImg;

	UPROPERTY(meta=(BindWidget))
	UImage* JumpscareMedia;
	// 현재 선택된 보더(밑줄)
	UPROPERTY()
	UBorder* CurrentBorder;
	
	// 현재 어느 창이 띄워진 상태인 지 구분할 변수
	UIState CurrentState = UIState::Animatronics;

	// 애니매트로닉스 사진 번호
	int32 AnimatronicsNum = 0;

	// 점프스케어 영상 번호
	int32 JumpscaresNum = 0;

	// 바인딩 시킬 버튼 함수
	// ANIMATRONICS
	UFUNCTION()
	void OnAnimatronics();

	// JUMPSCARES
	UFUNCTION()
	void OnJumpScares();

	// Left, Right Button
	UFUNCTION()
	void OnLeft();
	UFUNCTION()
	void OnRight();

	// NIGHTMARE
	UFUNCTION()
	void OnNightmare();

	// NIGHTMARE->START
	UFUNCTION()
	void OnStart();

	// CHEATS
	UFUNCTION()
	void OnCheats();

	// CHEATS->HOUSEMAP
	UFUNCTION()
	void OnHouseMap();

	// CHEATS->FASTNIGHTS
	UFUNCTION()
	void OnFastNights();

	void ChangeImg(int32 num);

	// 해당 State를 켜주는 함수
	void StateOn(UIState State);

	// 해당 State를 꺼주는 함수
	void StateOff(UIState State);

	// State 변경 함수
	void SetState(UIState NextState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
    class UMediaPlayer* MediaPlayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
    class UMediaSource* MediaSourceFreddy;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
    class UMediaSource* MediaSourceBonnie;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
    class UMediaSource* MediaSourceChica;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
    class UMediaSource* MediaSourceFoxy;
	
	UPROPERTY()
	TArray<UMediaSource*> MediaSources;

    UFUNCTION(BlueprintCallable, Category = "Media")
    void PlayVideo(int32 Num);

	UPROPERTY(EditAnywhere)
	class USoundBase* Audio;

	UPROPERTY()
	class UAudioComponent* AudioComp;

	UFUNCTION()
	void SaveHouseMap(bool Value);
	UFUNCTION()
	void SaveFastNight(bool Value);
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FString MySlotName = TEXT("bCheat");
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 UserIndex = 0;

	UPROPERTY()
	class UbCheatSaveGame* SaveGameIns;
};
