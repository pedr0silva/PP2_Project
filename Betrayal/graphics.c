#include "Include\Structs.h"
#include "Graphics.h"
#include <conio.h>
#include <Windows.h>

BOOL ShowConsoleCursor(BOOL showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
	return TRUE;
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
BOOL InputBreak()
{
	getch();
	return TRUE;
}
string* InsertText(string infoText, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	string auxStr;
	InsertLineInDrawingTable(*drawingTable, 0, MAX_HEIGHT - 1, infoText);
	DrawMap(*drawingTable);

	printf("\n");
	ShowConsoleCursor(TRUE);
	do
	{
		gets(auxStr);
	}while (strlen(auxStr) == 0);
	ShowConsoleCursor(FALSE);
	return auxStr;
}
int InsertNumber(string infoText, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	int auxInt;

	InsertLineInDrawingTable(*drawingTable, 0, MAX_HEIGHT - 1, infoText);
	DrawMap(*drawingTable);

	printf("\n");
	ShowConsoleCursor(TRUE);
	do
	{
		scanf("%d", &auxInt);
	} while (auxInt < 0);
	ShowConsoleCursor(FALSE);

	return auxInt;
}


BOOL InsertSelectableText(string text, int x, int y, int currentSelected, int movingSelected, int movedX, int movedY, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	Vector2 auxPos = ChangeVector2(x, y);
	if (currentSelected == movingSelected)
		auxPos = ChangeVector2(movedX, movedY);
	InsertLineInDrawingTable(*drawingTable, auxPos.x, auxPos.y, text);

	return TRUE;
}
BOOL InsertLineInDrawingTable(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH], int x, int y, string text)
{
	int lenght = strlen(text);
	if (x > MAX_WIDTH || y > MAX_HEIGHT || x < 0 || y < 0)
		return FALSE;
	for (int i = 0; i < lenght; i++)
	{
		(*drawingTable)[y][x++] = text[i];
		if (x > MAX_WIDTH)
		{
			x = 0;
			y++;
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
	return TRUE;
}
Vector2 WorldToCameraPos(CameraPtr camera, Vector2 worldPosition)
{
	Vector2 result;
	result.x = worldPosition.x - camera->MinBound.x;
	result.x = worldPosition.y - camera->MinBound.y;

	return result;
}
BOOL IsInCameraView(CameraPtr camera, Vector2 worldPosition)
{
	int x = worldPosition.x - camera->MinBound.x;
	int y = worldPosition.y - camera->MinBound.y;
	if (x > 0 && x < MAX_WIDTH && y > 0 && y < MAX_HEIGHT)
		return TRUE;
	return FALSE;
}
Camera InitCamera(int x, int y)
{
	Camera camera;
	camera.MinBound.x = x;
	camera.MinBound.y = y;

	camera.minLenght = x + y;

	CleanDrawingTable(camera.viewPort);
	return camera;
}




int DrawRoom(RoomPtr room, CameraPtr camera)
{
	if (strcmp(room->name, "MAIN ROOM" == 0))
	{

	}
	else
	{
		Vector2 screenPos;
		screenPos.x = camera->MinBound.x - room->position.x;
		screenPos.y = camera->MinBound.y - room->position.y;

		string aux;

		if (room->wall[0].WallType == DOOR)
			strcpy(aux, "## ##");
		if (room->wall[0].WallType == EMPTY)
			strcpy(aux, "#####");
		if (room->wall[0].WallType == WINDOW)
			strcpy(aux, "##+##");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, aux);

		strcpy(aux, "#   #");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, aux);


		screenPos.y++;
		strcpy(aux, "#   #");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, aux);
		screenPos.y++;
		if (room->wall[2].WallType == DOOR)
			strcpy(aux, "## ##");
		if (room->wall[2].WallType == EMPTY)
			strcpy(aux, "#####");
		if (room->wall[2].WallType == WINDOW)
			strcpy(aux, "##+##");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y, aux);
	}
	return 1;
}
int UpdateMap(FloorPtr currentFloor, CameraPtr camera, Vector2 cameraMovement)
{
	camera->MinBound.x += cameraMovement.x;
	camera->MinBound.y += cameraMovement.y;
	camera->minLenght = camera->MinBound.x + camera->MinBound.y;

	RoomPtr roomAux = currentFloor->roomList;
	while (roomAux)
	{
		if (roomAux->positionLenght < camera->minLenght)
		{
			roomAux = roomAux->next;
			continue;
		}
		/*if (roomAux->positionLenght > camera->maxLenght)
			break;*/
		DrawRoom(roomAux, camera);
		roomAux = roomAux->next;
	}
	//DRAW PLAYERS AND WATNOT
}
int DrawMap(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	for (int i = 0; i < MAX_HEIGHT; i++)
		for (int j = 0; j < MAX_WIDTH; j++)
			cprintf("%c", (*drawingTable)[i][j]);
}
BOOL DrawError(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	InsertLineInDrawingTable(*drawingTable, 0, MAX_HEIGHT - 1, "WRONG INPUT PLEASE TRY AGAIN");
	return TRUE;
}




void Create_Data_Menu(MasterPtr master, string type, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	string auxStr;
	if (strcmp(type, "CHARACTERS") == 0)
	{
		int might, speed, sanity, inteligence;
		CharacterPtr auxChar;

		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, type);
		strcpy(auxStr, InsertText("INSERT NAME:", drawingTable));

		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, type);
		might = InsertNumber("INSERT MIGHT:", drawingTable);

		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, type);
		speed = InsertNumber("INSERT SPEED:", drawingTable);

		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, type);
		sanity = InsertNumber("INSERT SANITY:", drawingTable);

		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, type);
		inteligence = InsertNumber("INSERT INTELIGENCE:", drawingTable);

		auxChar = CreateChar(auxStr, might, speed, sanity, inteligence);
		AddCharToArray(master, auxChar);
	}
}
void Delete_Data_Menu(MasterPtr master, string type, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	Vector2 topListVec;
	Camera camera = InitCamera(0, 0);
	unsigned int selected = 0;
	int counter;
	do
	{
		CleanDrawingTable(*drawingTable);

		InsertLineInDrawingTable(*drawingTable, 10, 3, "DELETE");

		topListVec = ChangeVector2(35, 16);
		counter = 0;
		topListVec.y -= (selected * 2);
		for (int i = 0; i < master->cards.charCount; i++)
		{
			if (IsInCameraView(&camera, topListVec) == TRUE)
				InsertSelectableText(master->cards.characterList[i].name, topListVec.x, topListVec.y, selected, counter, 38, 16, drawingTable);
			topListVec.y += 2;
			counter++;
		}
		InsertSelectableText("BACK", topListVec.x, topListVec.y, selected, counter, 38, 16, drawingTable);

		DrawMap(*drawingTable);

		input = ReadInput();

		if (input == DOWN_ARROW && selected < master->cards.charCount)
		{
			selected++;
			camera = InitCamera(camera.MinBound.x, ++camera.MinBound.y);
		}
		else if (input == UP_ARROW && selected > 0)
		{
			selected--;
			camera = InitCamera(camera.MinBound.x, --camera.MinBound.y);
		}
		else if (input == ENTER && selected < counter)
		{
			RemoveCharFromArray(master, master->cards.characterList[selected].name);
		}
	} while (!((input == ENTER) && (selected == counter)));
}
void Manage_Data_Menu(MasterPtr master, string type, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	unsigned int selected = 0;
	do
	{
		CleanDrawingTable(*drawingTable);

		InsertLineInDrawingTable(*drawingTable, 10, 3, type);
		InsertSelectableText("CREATE", 35, 16, selected, 0, 38, 16, drawingTable);
		InsertSelectableText("EDIT", 35, 18, selected, 1, 38, 18, drawingTable);
		InsertSelectableText("DELETE", 35, 20, selected, 2, 38, 20, drawingTable);
		InsertSelectableText("BACK", 35, 22, selected, 3, 38, 22, drawingTable);

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
				Create_Data_Menu(master, type, drawingTable);
				break;
			case 1:
				break;
			case 2:
				Delete_Data_Menu(master, type, drawingTable);
				break;
			default:
				break;
			}
		}

	} while (!((input == ENTER) && (selected == 3)));
}
void Reset_Menu(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	unsigned int selected = 0;

	do
	{
		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, "RESET ALL DATA");
		InsertLineInDrawingTable(*drawingTable, 35, 14, "ARE YOU SURE YOU WANT TO RESET ALL DATA?");

		InsertSelectableText("YES", 35, 16, selected, 0, 38, 16, drawingTable);
		InsertSelectableText("NO", 35, 18, selected, 1, 38, 18, drawingTable);

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
				Reset(master);
				break;
			default:
				break;
			}
		}
	} while (input != ENTER);
}
void Credits(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	do
	{
		CleanDrawingTable(*drawingTable);

		InsertLineInDrawingTable(*drawingTable, 10, 3, "CREDITS");

		InsertLineInDrawingTable(*drawingTable, 35, 14, "PROGRAMMED BY:");
		InsertLineInDrawingTable(*drawingTable, 35, 16, "DIOGO PORTELA");
		InsertLineInDrawingTable(*drawingTable, 35, 18, "PEDRO SILVA");

		InsertLineInDrawingTable(*drawingTable, 40, 20, "BACK");

		DrawMap(*drawingTable);

		input = ReadInput();

	} while (input != ENTER);
}
void Options(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	unsigned int selected = 0;

	do
	{
		CleanDrawingTable(*drawingTable);

		InsertLineInDrawingTable(*drawingTable, 10, 3, "OPTIONS");

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
				Manage_Data_Menu(master, "CHARACTERS", drawingTable);
				break;
			case 1:
				Manage_Data_Menu(master, "EVENTS", drawingTable);
				break;
			case 2:
				Manage_Data_Menu(master, "ITEMS", drawingTable);
				break;
			case 3:
				Manage_Data_Menu(master, "MINIONS", drawingTable);
				break;
			case 4:
				Manage_Data_Menu(master, "OMENS", drawingTable);
				break;
			case 5:
				Manage_Data_Menu(master, "ROOMS", drawingTable);
				break;
			case 6:
				Reset_Menu(master, drawingTable);
			default:
				break;
			}
		}

	} while (!((input == ENTER) && (selected == 7)));
}
void Menu(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	unsigned int selected = 0;

	CleanDrawingTable(*drawingTable);
	InsertLineInDrawingTable(*drawingTable, 40, 14, "WELCOME TO BETRAYAL AT HOUSE ON THE HILL");

	DrawMap(*drawingTable);

	InputBreak();
	do
	{
		CleanDrawingTable(*drawingTable);

		InsertLineInDrawingTable(*drawingTable, 10, 3, "MAIN MENU");

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
	} while (!((input == ENTER) && (selected == 3)));
	return 1;
}