// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/TimeUI.h"
#include "Components/TextBlock.h"

void UTimeUI::SetTime(FString Value)
{
	Time->SetText(FText::FromString(Value));
}
