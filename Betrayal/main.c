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

	/*boolean start = LoadMaster(&GameMaster);
	if (start == FALSE)
	{
		printf("%s", "THERE HAS BEEN AN ERROR LOADING.\n");
		InputBreak();
	}*/

	InputBreak();

	CleanDrawingTable(&DrawingTable);

	Vector2Ptr x = CreateVector2(10,10);
	string z = "WATUP";
	
	//InsertLineInDrawingTable(&DrawingTable, x, &z);
	DrawMap(&DrawingTable);

	//Menu(&GameMaster, &DrawingTable);
	
	Reset(&GameMaster);

	boolean exit = EndMaster(&GameMaster);
	if (exit == FALSE)
	{
		printf("%s", "THERE HAS BEEN AN ERROR SAVING.\n");
		InputBreak();
	}

	return 0;
}