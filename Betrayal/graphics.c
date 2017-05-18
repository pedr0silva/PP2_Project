#include "estructs.h"
#include <conio.h>


void ReadInput(string input)
{
	printf("INPUT: ");
	gets(input);
	strupr(input);
}
void InputBreak()
{
	printf("PRESS A KEY TO CONTINUE.");
	getch();
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
	system("cls");
	for (int i = 0; i < MAX_HEIGHT; i++)
		for (int j = 0; j < MAX_WIDTH; j++)
			(*drawingTable)[i][j] = ' ';
}

int UpdateMap(Master *mainStruct, char *drawingTable[MAX_HEIGHT][MAX_WIDTH])
{

}
int DrawMap(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	for (int i = 0; i < MAX_HEIGHT; i++)
		for (int j = 0; j < MAX_WIDTH; j++)
			printf("%c", (*drawingTable)[i][j]);
}

void Options(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	string inputStr;
	InitString(inputStr);

	string auxStr;
	Vector2Ptr auxPos = CreateVector2(0, 0);

	do
	{
		CleanDrawingTable(*drawingTable);

		strcpy(auxStr, "CHARACTERS");
		auxPos = ChangeVector2(auxPos, 35, 14);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "BACK");
		auxPos = ChangeVector2(auxPos, 35, 16);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		DrawMap(*drawingTable);
		ReadInput(inputStr);

	} while ((strcmp(inputStr, "BACK") != 0));
}

void Menu(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	string inputStr;
	string auxStr;
	Vector2Ptr auxPos = CreateVector2(40, 14);
	boolean error = FALSE;
	InitString(inputStr);

	strcpy(auxStr, "WELCOME TO BETRAYAL AT HOUSE ON THE HILL");
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

		if (error == TRUE)
		{
			strcpy(auxStr, "WRONG INPUT PLEASE TRY AGAIN");
			auxPos = ChangeVector2(auxPos, 0, MAX_HEIGHT - 1);
			InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);
			error = FALSE;
		}

		DrawMap(*drawingTable);
		ReadInput(inputStr);

		if (strcmp(inputStr, "PLAY") == 0)
			break;
		//Play(master, drawingTable);
		else if (strcmp(inputStr, "OPTIONS") == 0)
			Options(master, drawingTable);
		else if (strcmp(inputStr, "EXIT") != 0)
			error = TRUE;
	
	} while ((strcmp(inputStr, "EXIT") != 0));

	CleanDrawingTable(*drawingTable);
	DrawMap(*drawingTable);

	auxPos = DestroyVector2(auxPos);
	return 1;
}