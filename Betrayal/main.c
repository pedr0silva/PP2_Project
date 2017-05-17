/*
File: main.c
Description:
14/04/2017 - This file will be the playground for a while.
------------//---------------//------------//---------------//------------//---------------
Name: Pedro Silva
Email: pmiguelfs@gmail.com
Date Log:
------------//---------------//------------//---------------//------------//---------------
Name: Diogo Portela
Email:
Date Log:
*/

#include "estructs.h"

int main(void)
{
	//Important variables.
	Master GameMaster;
	char DrawingTable[MAX_HEIGHT][MAX_WIDTH];

	string xx = "hello";
	RoomPtr x = CreateRoom(&xx, NULL, NULL, EMPTY, EMPTY, EMPTY, EMPTY);
	FloorPtr xxx = CreateFloor(BASEMENT);
	xxx->roomList = AddRoomToList(xxx->roomList, x);

	xxx = DestroyFloor(xxx);

	boolean start = LoadMaster(&GameMaster);

	int aux = Menu();
	
	boolean exit = WriteCards(&(GameMaster.cards));

	return 0;
}