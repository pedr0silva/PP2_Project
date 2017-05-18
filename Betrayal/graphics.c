#include "estructs.h"


void ShowConsoleCursor(BOOL showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
KEYBOARD ReadInput()
{
	KEYBOARD key = NONE;
	char input;

	input = getch();
	switch (input)
	{
	case -32:
	{
		input = getch();
		switch (input)
		{
		case UP_ARROW:
			return UP_ARROW;
		case DOWN_ARROW:
			return DOWN_ARROW;
		case LEFT_ARROW:
			return LEFT_ARROW;
		case RIGHT_ARROW:
			return RIGHT_ARROW;
		default:
			return NONE;
		}
	}
	case ESC:
		return ESC;
	case SPACE:
		return SPACE;
	case ENTER:
		return ENTER;
	default:
		return NONE;
	}
}
void InputBreak()
{
	getch();
}

BOOL InsertSelectableText(string text, int x, int y, int currentSelected, int movingSelected, int movedX, int movedY, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	Vector2Ptr auxPos = CreateVector2(x, y);
	BOOL auxBool = TRUE;
	if (currentSelected == movingSelected)
		auxBool = ChangeVector2(auxPos, movedX, movedY);
	InsertLineInDrawingTable(*drawingTable, auxPos, text);

	auxPos = DestroyVector2(auxPos);
	return TRUE;
}
BOOL InsertLineInDrawingTable(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH], Vector2Ptr position, string text)
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
BOOL CleanDrawingTable(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
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
			cprintf("%c", (*drawingTable)[i][j]);
}
void DrawError(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	string auxStr = "WRONG INPUT PLEASE TRY AGAIN";
	Vector2Ptr auxPos = CreateVector2(0, MAX_HEIGHT - 1);

	InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);
}

void Reset_Menu(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	unsigned int selected = 0;
	string auxStr;
	Vector2Ptr auxPos = CreateVector2(0, 0);

	do
	{
		CleanDrawingTable(*drawingTable);

		strcpy(auxStr, "ARE YOU SURE YOU WANT TO RESET ALL DATA?");
		ChangeVector2(auxPos, 35, 14);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "YES");
		ChangeVector2(auxPos, 35, 16);
		if (selected == 0)
			ChangeVector2(auxPos, 38, 16);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "NO");
		ChangeVector2(auxPos, 35, 18);
		if (selected == 1)
			ChangeVector2(auxPos, 38, 18);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		DrawMap(*drawingTable);

		input = ReadInput();

		if (input == DOWN_ARROW && selected < 1)
			selected++;
		else if (input == UP_ARROW && selected > 0)
			selected--;
		else if (input == ENTER)
		{
			switch (selected)
			{
			case 0:
				break;
			default:
				break;
			}
		}
	} while (1);

	CleanDrawingTable(*drawingTable);
	DrawMap(*drawingTable);

	auxPos = DestroyVector2(auxPos);
}
void Credits(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	string auxStr;
	Vector2Ptr auxPos = CreateVector2(0, 0);

	do
	{
		CleanDrawingTable(*drawingTable);


		strcpy(auxStr, "CREATES BY:");
		ChangeVector2(auxPos, 35, 14);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "DIOGO PORTELA");
		ChangeVector2(auxPos, 35, 16);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "PEDRO SILVA");
		ChangeVector2(auxPos, 35, 18);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		strcpy(auxStr, "BACK");
		ChangeVector2(auxPos, 40, 20);
		InsertLineInDrawingTable(*drawingTable, auxPos, auxStr);

		DrawMap(*drawingTable);

		input = ReadInput();

	} while (input != ENTER);

	CleanDrawingTable(*drawingTable);
	DrawMap(*drawingTable);

	auxPos = DestroyVector2(auxPos);
}
void Options(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	unsigned int selected = 0;

	do
	{
		CleanDrawingTable(*drawingTable);

		InsertSelectableText("CHARACTERS", 35, 8, selected, 0, 38, 8, drawingTable);
		InsertSelectableText("EVENTS", 35, 10, selected, 1, 38, 10, drawingTable);
		InsertSelectableText("ITEMS", 35, 12, selected, 2, 38, 12, drawingTable);
		InsertSelectableText("MINIONS", 35, 14, selected, 3, 38, 14, drawingTable);
		InsertSelectableText("OMENS", 35, 16, selected, 4, 38, 16, drawingTable);
		InsertSelectableText("ROOMS", 35, 18, selected, 5, 38, 18, drawingTable);
		InsertSelectableText("RESET", 35, 20, selected, 6, 38, 20, drawingTable);
		InsertSelectableText("BACK", 35, 22, selected, 7, 38, 22, drawingTable);

		DrawMap(*drawingTable);

		input = ReadInput();

		if (input == DOWN_ARROW && selected < 7)
			selected++;
		else if (input == UP_ARROW && selected > 0)
			selected--;
		else if (input == ENTER)
		{
			switch (selected)
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			default:
				break;
			}
		}

	} while (!((input == ENTER) && (selected == 7)));

	CleanDrawingTable(*drawingTable);
	DrawMap(*drawingTable);
}
void Menu(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	unsigned int selected = 0;
	Vector2 auxPos;
	ChangeVector2(&auxPos, 40, 14);

	InsertLineInDrawingTable(*drawingTable, &auxPos, "WELCOME TO BETRAYAL AT HOUSE ON THE HILL");

	DrawMap(*drawingTable);

	InputBreak();
	do
	{
		CleanDrawingTable(*drawingTable);

		InsertSelectableText("NEW GAME", 35, 14, selected, 0, 38, 14, drawingTable);
		InsertSelectableText("OPTIONS", 35, 16, selected, 1, 38, 16, drawingTable);
		InsertSelectableText("CREDITS", 35, 18, selected, 2, 38, 18, drawingTable);
		InsertSelectableText("EXIT", 35, 20, selected, 3, 38, 20, drawingTable);

		DrawMap(*drawingTable);
		
		input = ReadInput();

		if (input == DOWN_ARROW && selected < 3)
			selected++;
		else if (input == UP_ARROW && selected > 0)
			selected--;
		else if (input == ENTER)
		{
			switch (selected)
			{
			case 0:
				break;
			case 1:
				Options(master, drawingTable);
				break;
			case 2:
				Credits(master, drawingTable);
			default:
				break;
			}
		}		
	} while(!((input == ENTER) && (selected == 3)));

	CleanDrawingTable(*drawingTable);
	DrawMap(*drawingTable);
	return 1;
}