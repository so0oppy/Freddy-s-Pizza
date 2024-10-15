// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/MinimapUI.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
void UMinimapUI::UpdateRoom(int32 RoomNum , int32 EnemyCase ,int32 Mode)
{
	
	// 윤선모드
	if ( Mode == 0 )
	{
		switch ( RoomNum )
		{
		case 0:
			MoveEnemyToRoom(Bonnie , Room7_0);
			break;
		case 1:
			MoveEnemyToRoom(Bonnie , Room5_0);
			break;
		case 2:
			MoveEnemyToRoom(Bonnie , Room2_0);
			break;
		case 3:
			MoveEnemyToRoom(Bonnie , Room1_0);
			break;

		}
	}
	else
	{
	// 수빈모드 : 1은 치카, 2는 폭시
		switch ( RoomNum )
		{
		case 1:
			if ( EnemyCase == 1 )
			{
				MoveEnemyToRoom(Chica, Room1_0);
			}
			else if ( EnemyCase == 2 )
			{
				MoveEnemyToRoom(Foxy , Room1_0);
			}
			break;
		case 2:
			if ( EnemyCase == 1 )
			{
				MoveEnemyToRoom(Chica , Room2_0);
			}
			else if ( EnemyCase == 2 )
			{
				MoveEnemyToRoom(Foxy , Room2_0);
			}
			break;
		case 3:
			if ( EnemyCase == 1 )
			{
				MoveEnemyToRoom(Chica , Room3_0);
			}
			else if ( EnemyCase == 2 )
			{
				MoveEnemyToRoom(Foxy , Room3_0);
			}
			break;
		case 4:
			if ( EnemyCase == 1 )
			{
				MoveEnemyToRoom(Chica , Room4_0);
			}
			else if ( EnemyCase == 2 )
			{
				MoveEnemyToRoom(Foxy , Room4_0);
			}
			break;
		case 5:
			if ( EnemyCase == 1 )
			{
				MoveEnemyToRoom(Chica , Room5_0);
			}
			else if ( EnemyCase == 2 )
			{
				MoveEnemyToRoom(Foxy , Room5_0);
			}
			break;
		case 6:
			if ( EnemyCase == 1 )
			{
				MoveEnemyToRoom(Chica , Room6_0);
			}
			else if ( EnemyCase == 2 )
			{
				MoveEnemyToRoom(Foxy , Room6_0);
			}
			break;
		case 7:
			if ( EnemyCase == 1 )
			{
				MoveEnemyToRoom(Chica , Room7_0);
			}
			else if ( EnemyCase == 2 )
			{
				MoveEnemyToRoom(Foxy , Room7_0);
			}
			break;
		case 8:
			if ( EnemyCase == 1 )
			{
				MoveEnemyToRoom(Chica , Room8_0);
			}
			else if ( EnemyCase == 2 )
			{
				MoveEnemyToRoom(Foxy , Room8_0);
			}
			break;
		case 9:
			if ( EnemyCase == 1 )
			{
				MoveEnemyToRoom(Chica , Room9_0);
			}
			else if ( EnemyCase == 2 )
			{
				MoveEnemyToRoom(Foxy , Room9_0);
			}
			break;
		case 10:
			if ( EnemyCase == 1 )
			{
				MoveEnemyToRoom(Chica , Room10_0);
			}
			else if ( EnemyCase == 2 )
			{
				MoveEnemyToRoom(Foxy , Room10_0);
			}
			break;
		}
	}
}

void UMinimapUI::MoveEnemyToRoom(UBorder* Enemy , UCanvasPanel* TargetRoom)
{
	if ( Enemy && TargetRoom )
	{
		if ( Enemy->GetParent() == Cast<UWidget>(TargetRoom) )
		{
			return;
		}
		UCanvasPanelSlot* CurrentSlot = Cast<UCanvasPanelSlot>(Enemy->Slot);
		Enemy->RemoveFromParent();
		TargetRoom->AddChild(Enemy);

		FVector2D Position = CurrentSlot->GetPosition();
		FVector2D Size = CurrentSlot->GetSize();

		UCanvasPanelSlot* NewSlot = Cast<UCanvasPanelSlot>(Enemy->Slot);
		if ( NewSlot )
		{
			// Set the position and size to the saved values
			NewSlot->SetPosition(Position);
			NewSlot->SetSize(Size);
		}
	}
}
