// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/CameraBlinkUI.h"

void UCameraBlinkUI::PlayBlinkAnim()
{
	int32 Rand = FMath::RandRange(0,2);

	switch ( Rand )
	{
	case 0:
		PlayAnimation(CameraBlink_1);
		break;
	case 1:
		PlayAnimation(CameraBlink_2);
		break;
	case 2:
		PlayAnimation(CameraBlink_3);
		break;

	}
}
