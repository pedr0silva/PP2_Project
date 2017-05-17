#include "estructs.h"
#include <conio.h>


void ReadInput(string input)
{
	printf("Input: ");
	gets(input);
	strupr(input);
	system("cls");
}
void InputBreak()
{
	printf("Press a key to continue.");
	getch();
	system("cls");
}

boolean InsertLineInDrawingTable(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH], Vector2Ptr position, string text)
{
	Vector2 aux = *position;
	int lenght = strlen(text);
	if (position->x > MAX_WIDTH || position->y > MAX_HEIGHT || position->x < 0 || position->y < 0)
		return FALSE;
	for (int i = 0; i < lenght; i++)
	{
		(*drawingTable)[aux.y][aux.x++] = text[i];
		if (aux.x > MAX_WIDTH)
		{
			aux.x = 0;
			aux.y++;
		}

	}
	return TRUE;
}
boolean CleanDrawingTable(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	for (int i = 0; i < MAX_HEIGHT; i++)
		for (int j = 0; j < MAX_WIDTH; j++)
			(*drawingTable)[i][j] = ' ';
}

int UpdateMap(Master *mainStruct, char *drawingTable[MAX_HEIGHT][MAX_WIDTH])
{
	
}
int DrawMap(char (*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	for (int i = 0; i < MAX_HEIGHT; i++)
		for (int j = 0; j < MAX_WIDTH; j++)
			printf("%c", (*drawingTable)[i][j]);
}
void Menu(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	string inputStr;
	InitString(inputStr);
	string auxStr = "WELCOME TO BETRAYAL AT HOUSE ON THE HILL";

	Vector2Ptr auxPos = CreateVector2(40, 14);
	InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);
	DrawMap(*drawingTable);	

	InputBreak();
	do
	{
		CleanDrawingTable(*drawingTable);

		strcpy(auxStr, "PLAY");
		auxPos = ChangeVector2(auxPos, 35, 14);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "OPTIONS");
		auxPos = ChangeVector2(auxPos, 35, 16);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "EXIT");
		auxPos = ChangeVector2(auxPos, 35, 18);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		DrawMap(*drawingTable);
		ReadInput(inputStr);
	} while ((strcmp(inputStr, "EXIT") != 0));

	CleanDrawingTable(*drawingTable);
	DrawMap(*drawingTable);

	auxPos = DestroyVector2(auxPos);
	return 1;
}