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

#include "Graphics.h"
#include "Include\Structs.h"
int main(void)
{
	ShowConsoleCursor(FALSE);

	string aux = "cards";

	//Important variables.
	Master GameMaster;
	char DrawingTable[MAX_HEIGHT][MAX_WIDTH];

	BOOL start = LoadMaster(&GameMaster, aux);
	if (start == FALSE)
	{
		printf("%s", "THERE HAS BEEN AN ERROR LOADING.\n");
		InputBreak();
	}

	Menu(&GameMaster, &DrawingTable);

	BOOL exit = EndMaster(&GameMaster, aux);
	if (exit == FALSE)
	{
		printf("%s", "THERE HAS BEEN AN ERROR SAVING.\n");
		InputBreak();
	}

	return 0;
}