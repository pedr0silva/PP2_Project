#include "estructs.h"
#include <conio.h>

void ShowConsoleCursor(BOOL showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
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

_boolean InsertLineInDrawingTable(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH], Vector2Ptr position, string text)
{
	Vector2 aux = *position;
	int lenght = strlen(text);
	if (position->x > MAX_WIDTH || position->y > MAX_HEIGHT || position->x < 0 || position->y < 0)
		return _FALSE;
	for (int i = 0; i < lenght; i++)
	{
		(*drawingTable)[aux.y][aux.x++] = text[i];
		if (aux.x > MAX_WIDTH)
		{
			aux.x = 0;
			aux.y++;
		}

	}
	return _TRUE;
}
_boolean CleanDrawingTable(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
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
	ShowConsoleCursor(FALSE);
	for (int i = 0; i < MAX_HEIGHT; i++)
		for (int j = 0; j < MAX_WIDTH; j++)
			printf("%c", (*drawingTable)[i][j]);
	ShowConsoleCursor(TRUE);
}
void DrawError(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	string auxStr = "WRONG INPUT PLEASE TRY AGAIN";
	Vector2Ptr auxPos = CreateVector2(0, MAX_HEIGHT - 1);

	InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);
}

void Credits(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	string inputStr;
	string auxStr;
	Vector2Ptr auxPos = CreateVector2(0, 0);
	_boolean error = _FALSE;
	InitString(inputStr);

	do
	{
		CleanDrawingTable(*drawingTable);

		strcpy(auxStr, "CREATES BY:");
		auxPos = ChangeVector2(auxPos, 35, 14);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "DIOGO PORTELA");
		auxPos = ChangeVector2(auxPos, 40, 16);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "PEDRO SILVA");
		auxPos = ChangeVector2(auxPos, 40, 18);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "BACK");
		auxPos = ChangeVector2(auxPos, 35, 22);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		if (error == _TRUE)
		{
			DrawError(drawingTable);
			error = _FALSE;
		}

		DrawMap(*drawingTable);
		ReadInput(inputStr);

		if (strcmp(inputStr, "BACK") != 0)
			error = _TRUE;

	} while ((strcmp(inputStr, "BACK") != 0));

	CleanDrawingTable(*drawingTable);
	DrawMap(*drawingTable);

	auxPos = DestroyVector2(auxPos);
}
void Options(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	string inputStr;
	InitString(inputStr);

	string auxStr;
	Vector2Ptr auxPos = CreateVector2(0, 0);
	_boolean error = _FALSE;

	do
	{
		CleanDrawingTable(*drawingTable);

		strcpy(auxStr, "CHARACTERS");
		auxPos = ChangeVector2(auxPos, 35, 8);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "ROOMS");
		auxPos = ChangeVector2(auxPos, 35, 10);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "EVENTS");
		auxPos = ChangeVector2(auxPos, 35, 12);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "ITEMS");
		auxPos = ChangeVector2(auxPos, 35, 14);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "OMENS");
		auxPos = ChangeVector2(auxPos, 35, 16);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "MINIONS");
		auxPos = ChangeVector2(auxPos, 35, 18);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "RESET");
		auxPos = ChangeVector2(auxPos, 35, 20);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "BACK");
		auxPos = ChangeVector2(auxPos, 35, 22);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		if (error == _TRUE)
		{
			DrawError(drawingTable);
			error = _FALSE;
		}

		DrawMap(*drawingTable);
		ReadInput(inputStr);

		if (strcmp(inputStr, "BACK") != 0)
			error = _TRUE;

	} while ((strcmp(inputStr, "BACK") != 0));

	CleanDrawingTable(*drawingTable);
	DrawMap(*drawingTable);

	auxPos = DestroyVector2(auxPos);
}
void Menu(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	string inputStr;
	string auxStr;
	Vector2Ptr auxPos = CreateVector2(40, 14);
	_boolean error = _FALSE;
	InitString(inputStr);

	strcpy(auxStr, "WELCOME TO BETRAYAL AT HOUSE ON THE HILL");
	InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);
	DrawMap(*drawingTable);

	InputBreak();
	do
	{
		CleanDrawingTable(*drawingTable);

		strcpy(auxStr, "NEW GAME");
		auxPos = ChangeVector2(auxPos, 35, 14);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "OPTIONS");
		auxPos = ChangeVector2(auxPos, 35, 16);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "CREDITS");
		auxPos = ChangeVector2(auxPos, 35, 18);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "EXIT");
		auxPos = ChangeVector2(auxPos, 35, 20);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		if (error == _TRUE)
		{
			DrawError(drawingTable);
			error = _FALSE;
		}

		DrawMap(*drawingTable);
		ReadInput(inputStr);

		if (strcmp(inputStr, "NEW GAME") == 0)
			break;
		//Play(master, drawingTable);
		else if (strcmp(inputStr, "CREDITS") == 0)
			Credits(master, drawingTable);
		else if (strcmp(inputStr, "OPTIONS") == 0)
			Options(master, drawingTable);
		else if (strcmp(inputStr, "EXIT") != 0)
			error = _TRUE;

	} while ((strcmp(inputStr, "EXIT") != 0));

	CleanDrawingTable(*drawingTable);
	DrawMap(*drawingTable);

	auxPos = DestroyVector2(auxPos);
	return 1;
}