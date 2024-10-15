// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MoveButtonUI.generated.h"

/**
 * 
 */
UCLASS()
class PIZZA_API UMoveButtonUI : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void SetOwner(AActor* NewActor);

	virtual void NativeConstruct() override;
private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* Door;

	UFUNCTION()
	void Move();

	UPROPERTY()
    class ADoor* Owner;
};
