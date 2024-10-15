// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialUI.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UTutorialUI : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	class UBorder* MainGuide;
	UPROPERTY(meta=(BindWidget))
	class UBorder* MainGuideBed;
	UPROPERTY(meta=(BindWidget))
	class UBorder* DoorGuide;

	void SetDoor();

	void SetBed();

	void SetMain();

	void SetHidden();
};
