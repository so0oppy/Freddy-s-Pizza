// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapUI.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UMinimapUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateRoom(int32 RoomNum, int32 EnemyCase,int32 Mode);

private:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UCanvasPanel* Room1_0;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCanvasPanel* Room2_0;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCanvasPanel* Room3_0;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCanvasPanel* Room4_0;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCanvasPanel* Room5_0;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCanvasPanel* Room6_0;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCanvasPanel* Room7_0;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCanvasPanel* Room8_0;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCanvasPanel* Room9_0;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCanvasPanel* Room10_0;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UBorder* Bonnie;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UBorder* Chica;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UBorder* Foxy;

	void MoveEnemyToRoom(UBorder* Enemy , UCanvasPanel* TargetRoom);
};
