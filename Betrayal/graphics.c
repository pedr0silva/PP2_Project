#include "Include\Structs.h"
#include "Graphics.h"
#include <conio.h>
#include <Windows.h>

#pragma region FUNCOES A PASSAR PARA A DLL

//Asigns a character to a player
BOOL AssignPlayer(MasterPtr master, int playerNumber, CharacterPtr selectedChar)
{
	selectedChar->playerNumber = playerNumber + 1;
	master->characterList = AddCharToList(master->characterList, selectedChar);

	return TRUE;
}

#pragma endregion

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
	} while (strlen(auxStr) == 0);
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
	if (y > MAX_HEIGHT || y < 0)
		return FALSE;
	for (int i = 0; i < lenght; i++)
	{
		if (x < MAX_WIDTH && x >= 0)
		{
			(*drawingTable)[y][x] = text[i];
		}
		x++;
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

WallType SelectWallType(string msg, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	unsigned int selected = 0;

	do
	{
		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, msg);

		InsertSelectableText("EMPY", 35, 16, selected, 0, 38, 16, drawingTable);
		InsertSelectableText("DOOR", 35, 18, selected, 1, 38, 18, drawingTable);
		InsertSelectableText("WINDOW", 35, 20, selected, 2, 38, 20, drawingTable);

		DrawMap(*drawingTable);
		input = ReadInput();

		if (input == DOWN_ARROW && selected < 2)
			selected++;
		else if (input == UP_ARROW && selected > 0)
			selected--;
	} while (input != ENTER);

	switch (selected)
	{
	case 0:
		return EMPTY;
	case 1:
		return DOOR;
	default:
		return WINDOW;
	}
}
int DrawArray(MasterPtr master, string type, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	Vector2 topListVec;
	Camera camera = InitCamera(0, 0);
	unsigned int selected = 0;
	int counter;
	int typeCounterAux = 0;
	do
	{
		CleanDrawingTable(*drawingTable);

		InsertLineInDrawingTable(*drawingTable, 10, 3, "SELECT:");

		topListVec = ChangeVector2(35, 16);
		counter = 0;
		topListVec.y -= (selected * 2);

		if (strcmp(type, "CHARACTERS") == 0)
		{
			typeCounterAux = master->cards.charCount;
			for (int i = 0; i < typeCounterAux; i++)
			{
				if (IsInCameraView(&camera, topListVec) == TRUE)
					InsertSelectableText(master->cards.characterList[i].name, topListVec.x, topListVec.y, selected, counter, 38, 16, drawingTable);
				topListVec.y += 2;
				counter++;
			}
		}
		else if (strcmp(type, "EVENTS") == 0)
		{
			typeCounterAux = master->cards.eventCount;
			for (int i = 0; i < typeCounterAux; i++)
			{
				if (IsInCameraView(&camera, topListVec) == TRUE)
					InsertSelectableText(master->cards.eventList[i].name, topListVec.x, topListVec.y, selected, counter, 38, 16, drawingTable);
				topListVec.y += 2;
				counter++;
			}
		}
		else if (strcmp(type, "ITEMS") == 0)
		{
			typeCounterAux = master->cards.itemCount;
			for (int i = 0; i < typeCounterAux; i++)
			{
				if (IsInCameraView(&camera, topListVec) == TRUE)
					InsertSelectableText(master->cards.itemList[i].name, topListVec.x, topListVec.y, selected, counter, 38, 16, drawingTable);
				topListVec.y += 2;
				counter++;
			}
		}
		else if (strcmp(type, "MINIONS") == 0)
		{
			typeCounterAux = master->cards.minionCount;
			for (int i = 0; i < typeCounterAux; i++)
			{
				if (IsInCameraView(&camera, topListVec) == TRUE)
					InsertSelectableText(master->cards.minionList[i].name, topListVec.x, topListVec.y, selected, counter, 38, 16, drawingTable);
				topListVec.y += 2;
				counter++;
			}
		}
		else if (strcmp(type, "OMENS") == 0)
		{
			typeCounterAux = master->cards.omenCount;
			for (int i = 0; i < typeCounterAux; i++)
			{
				if (IsInCameraView(&camera, topListVec) == TRUE)
					InsertSelectableText(master->cards.omenList[i].name, topListVec.x, topListVec.y, selected, counter, 38, 16, drawingTable);
				topListVec.y += 2;
				counter++;
			}
		}
		else if (strcmp(type, "ROOMS") == 0)
		{
			typeCounterAux = master->cards.roomCount;
			for (int i = 0; i < typeCounterAux; i++)
			{
				if (IsInCameraView(&camera, topListVec) == TRUE)
					InsertSelectableText(master->cards.roomList[i].name, topListVec.x, topListVec.y, selected, counter, 38, 16, drawingTable);
				topListVec.y += 2;
				counter++;
			}
		}

		InsertSelectableText("NONE", topListVec.x, topListVec.y, selected, counter, 38, 16, drawingTable);

		DrawMap(*drawingTable);
		input = ReadInput();

		if (input == DOWN_ARROW && selected < typeCounterAux)
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
			return selected;
		}
	} while (!((input == ENTER) && (selected == counter)));
	return -1;
}
int DrawRoom(RoomPtr room, CameraPtr camera)
{
	Vector2 screenPos;
	screenPos.x = room->position.x - camera->MinBound.x;
	screenPos.y = room->position.y - camera->MinBound.y;

	string aux;

	if (strcmp(room->name, "MAIN ROOM 1") == 0)
	{
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, "##########");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, "#  ----  #");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, "#   --   #");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, "#        #");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y, "###    ###");
	}
	else if (strcmp(room->name, "MAIN ROOM 2") == 0)
	{
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, "###    ###");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, "#        #");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, "          ");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, "#        #");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y, "###    ###");
	}
	else if (strcmp(room->name, "MAIN ROOM 3") == 0)
	{
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, "###    ###");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, "#        #");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, "          ");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, "#        #");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y, "#+#....#+#");
	}
	else
	{
		if (room->wall[0].WallType == DOOR)
			strcpy(aux, "###    ###");
		else if (room->wall[0].WallType == EMPTY)
			strcpy(aux, "##########");
		else if (room->wall[0].WallType == WINDOW)
			strcpy(aux, "####++####");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, aux);

		strcpy(aux, "#        #");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, aux);

		if (room->wall[1].WallType == DOOR)
			strcpy(aux, "          ");
		else if (room->wall[1].WallType == EMPTY)
			strcpy(aux, "#        ");
		else if (room->wall[1].WallType == WINDOW)
			strcpy(aux, "+        ");
		if (room->wall[3].WallType == DOOR)
			strcat(aux, " ");
		else if (room->wall[3].WallType == EMPTY)
			strcat(aux, "#");
		else if (room->wall[3].WallType == WINDOW)
			strcat(aux, "+");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, aux);

		strcpy(aux, "#        #");
		InsertLineInDrawingTable(camera->viewPort, screenPos.x, screenPos.y++, aux);

		if (room->wall[2].WallType == DOOR)
			strcpy(aux, "###    ###");
		else if (room->wall[2].WallType == EMPTY)
			strcpy(aux, "##########");
		else if (room->wall[2].WallType == WINDOW)
			strcpy(aux, "####++####");
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

BOOL PlayerMovement(MasterPtr master, CharacterPtr currentPlayer, CameraPtr cam, KEYBOARD input, FloorPtr currentFloor)
{
	RoomPtr auxRoom = currentFloor->roomList;
	int auxMovement = currentPlayer->speed;
	Vector2 auxVec = ChangeVector2(0, 0);
	BOOL roomExists = FALSE;

	if (input == LEFT_ARROW)
	{
		while (auxRoom != NULL) //percorre todos os rooms da lista
		{
			//se existe 1 room posicionado a esquerda
			if (auxRoom->position.x == currentPlayer->room->position.x - ROOM_SIZE * 2)
			{
				roomExists = TRUE;
				//se o player ainda se pode movimentar subtrai 1 movement
				auxMovement--;
				currentPlayer->room = auxRoom;
				break;
			}
			auxRoom = auxRoom->next;
		}
		if (!roomExists)
		{
			//se nao existe nenhum room posicionado esse lugar, entao abre um.
			OpenRoom(master, currentFloor, currentPlayer->room, LEFT);
			currentPlayer->room = LastRoom(currentFloor->roomList);
			UpdateMap(currentFloor, &cam, auxVec);
			DrawMap(cam->viewPort);
		}
	}
	if (input == RIGHT_ARROW)
	{
		while (auxRoom != NULL) //percorre todos os rooms da lista
		{
			//se existe 1 room posicionado a esquerda
			if (auxRoom->position.x == currentPlayer->room->position.x + ROOM_SIZE * 2)
			{
				roomExists = TRUE;
				//se o player ainda se pode movimentar subtrai 1 movement
				auxMovement--;
				currentPlayer->room = auxRoom;
				break;
			}
			auxRoom = auxRoom->next;
		}
		if (!roomExists)
		{
			//se nao existe nenhum room posicionado esse lugar, entao abre um.
			OpenRoom(master, currentFloor, currentPlayer->room, RIGHT);
			currentPlayer->room = LastRoom(currentFloor->roomList);
			UpdateMap(currentFloor, &cam, auxVec);
			DrawMap(cam->viewPort);
		}
	}
	if (input == UP_ARROW)
	{
		while (auxRoom != NULL) //percorre todos os rooms da lista
		{
			//se existe 1 room posicionado a esquerda
			if (auxRoom->position.y == currentPlayer->room->position.y + ROOM_SIZE)
			{
				roomExists = TRUE;
				//se o player ainda se pode movimentar subtrai 1 movement
				auxMovement--;
				currentPlayer->room = auxRoom;
				break;
			}
			auxRoom = auxRoom->next;
		}
		if (!roomExists)
		{
			//se nao existe nenhum room posicionado esse lugar, entao abre um.
			OpenRoom(master, currentFloor, currentPlayer->room, UP);
			currentPlayer->room = LastRoom(currentFloor->roomList);
			UpdateMap(currentFloor, &cam, auxVec);
			DrawMap(cam->viewPort);
		}
	}
	if (input == DOWN_ARROW)
	{
		while (auxRoom != NULL) //percorre todos os rooms da lista
		{
			//se existe 1 room posicionado a esquerda
			if (auxRoom->position.y == currentPlayer->room->position.y - ROOM_SIZE)
			{
				roomExists = TRUE;
				//se o player ainda se pode movimentar subtrai 1 movement
				auxMovement--;
				currentPlayer->room = auxRoom;
				break;
			}
			auxRoom = auxRoom->next;
		}
		if (!roomExists)
		{
			//se nao existe nenhum room posicionado esse lugar, entao abre um.
			OpenRoom(master, currentFloor, currentPlayer->room, DOWN);
			currentPlayer->room = LastRoom(currentFloor->roomList);
			UpdateMap(currentFloor, &cam, auxVec);
			DrawMap(cam->viewPort);
		}
	}
	return TRUE;
}

void GameLoop(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	int stopGame = 1;
	KEYBOARD input = NONE;
	Camera cam = InitCamera(-((MAX_WIDTH / 2) - (ROOM_SIZE)), -((MAX_HEIGHT / 2) - (ROOM_SIZE * 3 / 2)));
	CharacterPtr currentPlayer;
	FloorPtr currentFloor = master->map.mapFloor->next;

	Vector2 auxVec = ChangeVector2(0, 0);
	RoomPtr roomAux = InstanciateRoom(&(master->cards.roomList[0]), auxVec);
	currentFloor->roomList = AddRoomToList(currentFloor->roomList, roomAux);

	auxVec = ChangeVector2(0, 0 + ROOM_SIZE);
	roomAux = InstanciateRoom(&(master->cards.roomList[1]), auxVec);
	currentFloor->roomList = AddRoomToList(currentFloor->roomList, roomAux);

	auxVec = ChangeVector2(0, 0 + ROOM_SIZE * 2);
	roomAux = InstanciateRoom(&(master->cards.roomList[2]), auxVec);
	currentFloor->roomList = AddRoomToList(currentFloor->roomList, roomAux);

	auxVec = ChangeVector2(0, 0);
	UpdateMap(master->map.mapFloor->next, &cam, auxVec);
	DrawMap(&(cam.viewPort));

	while (stopGame)
	{
		//UpdateMap(master->map.mapFloor->next, &cam, auxVec);
		input = ReadInput();
		RoomPtr auxRoom;
		CharacterPtr auxChar = master->characterList;
		BOOL roomExists = FALSE;

		while (auxChar != NULL)
		{
			int auxMovement = auxChar->speed;

			while (auxMovement > 0)
			{
				auxRoom = currentFloor->roomList;
				roomExists = FALSE;

				PlayerMovement(master, auxChar, &cam, input, currentFloor);
				auxChar = auxChar->next;
				input = NONE;
			}

		}
	}
}

//MENUS
void Create_Data_Menu(MasterPtr master, string type, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	string auxStr;
	string descriptionStr;
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
	else if (strcmp(type, "EVENTS") == 0)
	{
		int might, speed, sanity, inteligence;
		EventPtr auxEvent;

		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, type);
		strcpy(auxStr, InsertText("INSERT NAME:", drawingTable));

		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, type);
		strcpy(descriptionStr, InsertText("INSERT DESCRIPTIONS:", drawingTable));

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

		auxEvent = Create_Event(auxStr, descriptionStr, might, speed, sanity, inteligence);
		AddEventToArray(master, auxEvent);
	}
	else if (strcmp(type, "ITEMS") == 0)
	{
		int might, speed, sanity, inteligence;
		ItemPtr auxItem;

		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, type);
		strcpy(auxStr, InsertText("INSERT NAME:", drawingTable));

		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, type);
		strcpy(descriptionStr, InsertText("INSERT DESCRIPTIONS:", drawingTable));

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

		auxItem = CreateItem(auxStr, descriptionStr, might, speed, sanity, inteligence);
		AddItemToArray(master, auxItem);
	}
	else if (strcmp(type, "MINIONS") == 0)
	{
		int might, speed, sanity, inteligence;
		MinionPtr auxMinion;

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

		auxMinion = CreateMinion(auxStr, might, speed, sanity, inteligence);
		AddMinionToArray(master, auxMinion);
	}
	else if (strcmp(type, "OMENS") == 0)
	{
		int might, speed, sanity, inteligence;
		OmenPtr auxOmen;

		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, type);
		strcpy(auxStr, InsertText("INSERT NAME:", drawingTable));

		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, type);
		strcpy(descriptionStr, InsertText("INSERT DESCRIPTIONS:", drawingTable));

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

		auxOmen = CreateOmen(auxStr, descriptionStr, might, speed, sanity, inteligence);
		AddOmenToArray(master, auxOmen);
	}
	else if (strcmp(type, "ROOMS") == 0)
	{
		RoomPtr auxRoom;
		EventPtr eventSelectedPtr;
		OmenPtr omenSelectedPtr;

		CleanDrawingTable(*drawingTable);
		InsertLineInDrawingTable(*drawingTable, 10, 3, type);
		strcpy(auxStr, InsertText("INSERT NAME:", drawingTable));

		int eventSelected = DrawArray(master, "EVENTS", drawingTable);
		if (eventSelected > -1)
			eventSelectedPtr = &(master->cards.eventList[eventSelected]);
		else
			eventSelectedPtr = NULL;

		int omenSelected = DrawArray(master, "OMENS", drawingTable);
		if (omenSelected > -1)
			omenSelectedPtr = &(master->cards.omenList[omenSelected]);
		else
			omenSelectedPtr = NULL;

		WallType up = SelectWallType("Upper Wall", drawingTable);
		WallType left = SelectWallType("Left Wall", drawingTable);
		WallType down = SelectWallType("Down Wall", drawingTable);
		WallType right = SelectWallType("Right Wall", drawingTable);

		auxRoom = CreateRoom(auxStr, eventSelectedPtr, omenSelectedPtr, up, left, down, right);
		AddRoomToArray(master, auxRoom);
	}

}
void Edit_Data_Menu(MasterPtr master, string type, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{

}
void Delete_Data_Menu(MasterPtr master, string type, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	int result = DrawArray(master, type, drawingTable);
	if (result > -1)
	{
		if (strcmp(type, "CHARACTERS") == 0)
			RemoveCharFromArray(master, master->cards.characterList[result].name);
		else if (strcmp(type, "EVENTS") == 0)
			RemoveEventFromArray(master, master->cards.eventList[result].name);
		else if (strcmp(type, "ITEMS") == 0)
			RemoveItemFromArray(master, master->cards.itemList[result].name);
		else if (strcmp(type, "MINIONS") == 0)
			RemoveMinionFromArray(master, master->cards.minionList[result].name);
		else if (strcmp(type, "OMENS") == 0)
			RemoveOmentFromArray(master, master->cards.omenList[result].name);
		else if (strcmp(type, "ROOMS") == 0)
			RemoveRoomFromArray(master, master->cards.roomList[result].name);
	}
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
				Edit_Data_Menu(master, type, drawingTable);
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
void Create_Player_Menu(MasterPtr master, int numPlayers, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	string p = "CHARACTER SELECTION - PLAYER X";
	CharacterPtr aux = master->cards.characterList;
	string auxStr;
	KEYBOARD input = NONE;

	for (int playerNumber = 0, ASCIIconversion = 49; playerNumber < numPlayers; ASCIIconversion++, playerNumber++)
	{

		unsigned int selected = 0;

		p[29] = ASCIIconversion;

		do
		{
			CleanDrawingTable(*drawingTable);

			InsertLineInDrawingTable(*drawingTable, 10, 3, p);

			for (int j = 0, y = 8; j < numPlayers; j++, y += 2)  //terá espaco no ecra para tudo?
			{
				aux = &(master->cards.characterList[j]);
				sprintf(auxStr, "%s - %d, %d, %d, %d", aux->name, aux->might, aux->speed, aux->sanity, aux->inteligence);
				//deviamos eliminar ou arrajar outra forma de destacar os characters ja escolhidos.
				InsertSelectableText(auxStr, 35, y, selected, j, 38, y, drawingTable);
			}

			//1 linha para o back

			DrawMap(*drawingTable);

			input = ReadInput();

			if (input == DOWN_ARROW && selected < 12)
				selected++;
			else if (input == UP_ARROW && selected > 0)
				selected--;
			else if (input == ENTER)
			{
				AssignPlayer(master, playerNumber, &(master->cards.characterList[selected]));
			}
		} while ((input != ENTER));

	}
}
void Manage_Cards_Menu(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	unsigned int selected = 0;

	do
	{
		CleanDrawingTable(*drawingTable);

		InsertLineInDrawingTable(*drawingTable, 10, 3, "CARDS");

		InsertSelectableText("CHARACTERS", 35, 8, selected, 0, 38, 8, drawingTable);
		InsertSelectableText("EVENTS", 35, 10, selected, 1, 38, 10, drawingTable);
		InsertSelectableText("ITEMS", 35, 12, selected, 2, 38, 12, drawingTable);
		InsertSelectableText("MINIONS", 35, 14, selected, 3, 38, 14, drawingTable);
		InsertSelectableText("OMENS", 35, 16, selected, 4, 38, 16, drawingTable);
		InsertSelectableText("ROOMS", 35, 18, selected, 5, 38, 18, drawingTable);
		InsertSelectableText("BACK", 35, 20, selected, 6, 38, 20, drawingTable);

		DrawMap(*drawingTable);

		input = ReadInput();

		if (input == DOWN_ARROW && selected < 6)
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
			default:
				break;
			}
		}

	} while (!((input == ENTER) && (selected == 6)));
}
void Start(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	unsigned int selected = 0;

	do
	{
		CleanDrawingTable(*drawingTable);

		InsertLineInDrawingTable(*drawingTable, 10, 3, "SELECT THE NUMBER OF PLAYERS");

		InsertSelectableText("THREE", 35, 8, selected, 0, 38, 8, drawingTable);
		InsertSelectableText("FOUR", 35, 10, selected, 1, 38, 10, drawingTable);
		InsertSelectableText("FIVE", 35, 12, selected, 2, 38, 12, drawingTable);
		InsertSelectableText("SIX", 35, 14, selected, 3, 38, 14, drawingTable);
		InsertSelectableText("BACK", 35, 16, selected, 4, 38, 16, drawingTable);

		DrawMap(*drawingTable);

		input = ReadInput();

		if (input == DOWN_ARROW && selected < 4)
			selected++;
		else if (input == UP_ARROW && selected > 0)
			selected--;
		else if (input == ENTER)
		{
			switch (selected)
			{
			case 0:
				Create_Player_Menu(master, 3, drawingTable);
				GameLoop(master, drawingTable);
				break;
			case 1:
				Create_Player_Menu(master, 4, drawingTable);
				GameLoop(master, drawingTable);
				break;
			case 2:
				Create_Player_Menu(master, 5, drawingTable);
				GameLoop(master, drawingTable);
				break;
			case 3:
				Create_Player_Menu(master, 6, drawingTable);
				GameLoop(master, drawingTable);
				break;
			default:
				break;
			}
		}
	} while (!((input == ENTER) && (selected == 4)));
}
void Credits(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	KEYBOARD input = NONE;
	do
	{
		CleanDrawingTable(*drawingTable);

		InsertLineInDrawingTable(*drawingTable, 10, 3, "CREDITS");

		InsertLineInDrawingTable(*drawingTable, 35, 14, "PROGRAMMED BY:");
		InsertLineInDrawingTable(*drawingTable, 35, 16, "DIOGO PORTELA - EMAIL@GMAIL.COM");
		InsertLineInDrawingTable(*drawingTable, 35, 18, "PEDRO SILVA - PMIGUELFS@GMAIL.COM");

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

		InsertSelectableText("SAVE DATA", 35, 14, selected, 0, 38, 14, drawingTable);
		InsertSelectableText("LOAD DATA", 35, 16, selected, 1, 38, 16, drawingTable);
		InsertSelectableText("EDIT CURRENT DATA", 35, 18, selected, 2, 38, 18, drawingTable);
		InsertSelectableText("RESET DATA", 35, 20, selected, 3, 38, 20, drawingTable);
		InsertSelectableText("BACK", 35, 22, selected, 4, 38, 22, drawingTable);

		DrawMap(*drawingTable);

		input = ReadInput();

		if (input == DOWN_ARROW && selected < 4)
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
				Manage_Cards_Menu(master, drawingTable);
				break;
			case 3:
				Reset_Menu(master, drawingTable);
				break;
			default:
				break;
			}
		}

	} while (!((input == ENTER) && (selected == 4)));
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
				Start(master, drawingTable);
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