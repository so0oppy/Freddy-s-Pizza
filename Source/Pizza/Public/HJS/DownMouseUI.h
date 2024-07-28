// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DownMouseUI.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UDownMouseUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetPlayer(AFreddyPlayer* NewPlayer);
private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* MouseDown;

	virtual void NativeConstruct() override;

	class AFreddyPlayer* Player;
	UFUNCTION()
	void Down();
	UFUNCTION()
	void Up();


};
