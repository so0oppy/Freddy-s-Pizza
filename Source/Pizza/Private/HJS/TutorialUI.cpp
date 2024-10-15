// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/TutorialUI.h"
#include "Components/Border.h"
void UTutorialUI::NativeConstruct()
{
	
	DoorGuide->SetVisibility(ESlateVisibility::Hidden);
	MainGuide->SetVisibility(ESlateVisibility::HitTestInvisible);
	MainGuideBed->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UTutorialUI::SetDoor()
{
	// 문으로 갔을 때 세팅
	MainGuide->SetVisibility(ESlateVisibility::Hidden);
	DoorGuide->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UTutorialUI::SetBed()
{
	// 침대로 갔을 때 세팅
	MainGuide->SetVisibility(ESlateVisibility::Hidden);
}

void UTutorialUI::SetMain()
{
	// 메인으로 갔을 때 세팅
	MainGuide->SetVisibility(ESlateVisibility::HitTestInvisible);
	DoorGuide->SetVisibility(ESlateVisibility::Hidden);
}

void UTutorialUI::SetHidden()
{
	// 숨길 때 세팅
	SetVisibility(ESlateVisibility::Hidden);
}


