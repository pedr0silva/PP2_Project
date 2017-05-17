/*
File: functions.c
Description: This file has the needed functions to operate the data structures.
/!\ we may want to create various function files. ex: creator.c, listmanager.c, etc...
------------//---------------//------------//---------------//------------//---------------
Name: Pedro Silva
Email: pmiguelfs@gmail.com
Date Log:
14/04/2017 - Created a few basic functions to create maps, rooms and characters.

27/04/2017 - Worked on auxiliar functions to insert/modify the cards available in the game.

28/04/2017 - Added DiceRoll, Asign and Unasign Minion and Item functions.
		   - Added functions to save the game cards to a file.

01/05/2017 - Added LoadCards function.
------------//---------------//------------//---------------//------------//---------------
Name: Diogo Portela
Email:
Date Log:

*/

#include "estructs.h"

//Allocates memory for a Vector2 and gives value to its atributes.
Vector2Ptr CreateVector2(int x, int y)
{
	Vector2Ptr aux = (Vector2Ptr)malloc(sizeof(Vector2));
	aux->x = x;
	aux->y = y;
	return aux;
}
//Destroy Vector2 from memory.
Vector2Ptr DestroyVector2(Vector2Ptr node)
{
	free(node);
	return NULL;
}

//Initializes an empty String.
stringPtr InitString(stringPtr string)
{
	for (int i = 0; i < MAX_STRING; i++)
	{
		string[i] = 0;
	}
	return string;
}

//Allocates memory for a History and gives value to its atributes.
HistoryPtr CreateHistory(stringPtr text)
{
	HistoryPtr aux = (HistoryPtr)malloc(sizeof(History));
	aux->next = NULL;
	if (text != NULL)
		strcpy(aux->turn, strupr(text));
	else
		strcpy(aux->turn, "NULL");
	return aux;
}
//Adds a History to a HistoryList.
HistoryPtr AddHistoryToList(HistoryPtr head, HistoryPtr node)
{
	HistoryPtr aux = head;
	HistoryPtr aux2 = aux;


	if (head == NULL)
		head = node;
	else
	{
		while (aux &&  strcmp(aux->turn, node->turn) > 0)
		{
			aux2 = aux;
			aux = aux->next;
		}
		aux2->next = node;
		node->next = aux;
	}
	return head;
}
//Removes a History from a HistoryList.
HistoryPtr RemoveHistoryFromList(HistoryPtr head, HistoryPtr node)
{
	HistoryPtr aux = head;
	HistoryPtr aux2 = aux;
	if (head != NULL)
	{
		while (aux)
		{
			if (strcmp(aux->turn, node->turn) == 0)
			{
				aux2->next = aux->next;
				free(aux);
				break;
			}
			aux2 = aux;
			aux = aux->next;
		}
	}
	return head;
}
//Destroy History from memory.
HistoryPtr DestroyHistory(HistoryPtr node)
{
	free(node);
	return NULL;
}
//Destroy HistoryList from memory.
HistoryPtr DestroyHistoryList(HistoryPtr head)
{
	HistoryPtr aux = head;
	HistoryPtr aux2 = aux;
	while (aux)
	{
		aux = aux->next;
		aux2 = DestroyHistory(aux2);
		aux2 = aux;
	}
	return NULL;
}

#pragma region Rooms
//Assigns values to all four walls of a room.
boolean AssignWalls(RoomWallPtr arr, WallType upType, WallType leftType, WallType downType, WallType rightType)
{
	RoomWall aux;
	aux.Direction = UP;
	aux.WallType = upType;
	arr[0] = aux;

	aux.Direction = LEFT;
	aux.WallType = leftType;
	arr[1] = aux;

	aux.Direction = DOWN;
	aux.WallType = downType;
	arr[2] = aux;

	aux.Direction = RIGHT;
	aux.WallType = rightType;
	arr[3] = aux;

	return TRUE;
}
//Returns the wall of that direction in that room.
RoomWallPtr FindWallDirection(RoomPtr room, Direction direction)
{
	RoomWallPtr aux = NULL;
	if (direction == UP)
		aux = room->wall;
	else if (direction == LEFT)
		aux = &(room->wall[1]);
	else if (direction == DOWN)
		aux = &(room->wall[2]);
	else if (direction == RIGHT)
		aux = &(room->wall[3]);
	return aux;
}
//Copies the walls from one room to the other.
boolean CopyWalls(RoomWallPtr thisRoomWalls, RoomWallPtr otherRoomWalls)
{
	for (int i = 0; i < WALL_NUMBER; i++)
	{
		thisRoomWalls[i] = otherRoomWalls[i];
	}
	return TRUE;
}
//Rotates the walls of a room. Value > 0 clockwise; Value < 0 counter-clockwise.
RoomPtr RotateWalls(RoomPtr room, int value)
{
	if (value > 0)
	{
		RoomWall aux = room->wall[0];
		room->wall[0] = room->wall[1];
		room->wall[1] = room->wall[2];
		room->wall[2] = room->wall[3];
		room->wall[3] = aux;
	}
	if (value < 0)
	{
		RoomWall aux = room->wall[0];
		room->wall[0] = room->wall[3];
		room->wall[3] = room->wall[2];
		room->wall[2] = room->wall[1];
		room->wall[1] = aux;
	}

	return room;
}

//Allocates memory for a Room and gives value to its atributes.
RoomPtr CreateRoom(stringPtr roomName, EventPtr roomEvent, OmenPtr roomOmen, WallType upType, WallType leftType, WallType downType, WallType rightType)
{
	RoomPtr aux = (RoomPtr)malloc(sizeof(Room));

	if (aux != NULL)
	{
		aux->position.x = 0;
		aux->position.y = 0;
		aux->positionLenght = 0;
		if (roomName != NULL)
			strcpy(aux->name, strupr(roomName));
		else
			strcpy(aux->name, "NULL");
		aux->event = roomEvent;
		aux->omen = roomOmen;
		aux->next = NULL;
		boolean i = AssignWalls(&(aux->wall), upType, leftType, downType, rightType);
	}
	return aux;
}
//Creates a copy of a room to use in a game.
RoomPtr InstanciateRoom(RoomPtr room, Vector2 position)
{
	RoomPtr aux = (RoomPtr)malloc(sizeof(Room));
	aux->event = room->event;
	strcpy(aux->name, strupr(room->name));
	aux->omen = room->omen;
	aux->position = position;
	aux->positionLenght = aux->position.x + aux->position.y;
	aux->next = NULL;
	boolean i = CopyWalls(aux->wall, room->wall);

	if (i == TRUE)
		return aux;
	free(aux);
	return NULL;
}
//Adds a Room to a RoomList.
RoomPtr AddRoomToList(RoomPtr head, RoomPtr node)
{
	RoomPtr aux = head;
	RoomPtr aux2 = aux;


	if (head == NULL)
		head = node;
	else
	{
		while (aux && aux->positionLenght < node->positionLenght)
		{
			aux2 = aux;
			aux = aux->next;
		}
		aux2->next = node;
		node->next = aux;
	}
	return head;
}
//Add a Room to the RoomArray.
boolean AddRoomToArray(MasterPtr master, RoomPtr node)
{
	master->cards.roomList[master->cards.roomCount++] = *node;
	return TRUE;
}
//Removes a Room from the RoomArray.
boolean RemoveRoomFromArray(MasterPtr master, stringPtr node)
{
	RoomPtr aux = CreateRoom(NULL, NULL, NULL, EMPTY, EMPTY, EMPTY, EMPTY);
	for(int i = 0; i > master->cards.roomCount; i++)
		if (strcmp(master->cards.roomList[i].name, node) == 0)
		{
			for (int j = i; j > master->cards.roomCount - 1; i++)
				master->cards.roomList[j] = master->cards.roomList[j + 1];
			master->cards.roomList[master->cards.roomCount] = *aux;
			return TRUE;
		}
	return FALSE;
}
//Removes a Room from a RoomList.
RoomPtr RemoveRoomFromList(RoomPtr head, stringPtr name)
{
	RoomPtr aux = head;
	RoomPtr aux2 = aux;
	if (head != NULL)
	{
		while (aux)
		{
			if (strcmp(aux->name, name) == 0)
			{
				aux2->next = aux->next;
				free(aux);
				break;
			}
			aux2 = aux;
			aux = aux->next;
		}
	}
	return head;
}
//Destroy Room from memory.
RoomPtr DestroyRoom(RoomPtr room)
{
	free(room);
	return NULL;
}
//Destroy RoomList from memory.
RoomPtr DestroyRoomList(RoomPtr head)
{
	RoomPtr aux = head;
	RoomPtr aux2 = aux;
	while (aux)
	{
		aux = aux->next;
		aux2 = DestroyRoom(aux2);
		aux2 = aux;
	}
	return NULL;
}
//Returns a random room from the roomList.
RoomPtr RandomRoom(MasterPtr master)
{
	RoomPtr aux = NULL;
	int random = rand() % master->cards.roomCount;
	aux = &(master->cards.roomList[random]);
	return aux;
}
//Opens a Room and generates one.
boolean OpenRoom(MasterPtr master, FloorPtr floor, RoomPtr currentRoom, Direction direction)
{
	RoomWallPtr aux = FindWallDirection(currentRoom, direction);
	RoomPtr roomAux = NULL;
	RoomPtr randomRoom = NULL;

	if (aux->WallType == DOOR)
	{
		Vector2 addingValue = currentRoom->position;
		if (direction == UP)
			addingValue.y++;
		else if (direction == LEFT)
			addingValue.x--;
		else if (direction == DOWN)
			addingValue.y--;
		else if (direction == RIGHT)
			addingValue.x++;

		randomRoom = RandomRoom(master);

		roomAux = InstanciateRoom(randomRoom, addingValue);
		floor->roomList = AddRoomToList(floor->roomList, roomAux);
		return TRUE;
	}
	else
		return FALSE;
}
#pragma endregion

#pragma region Floor
//Allocates memory for a Floor and gives value to its atributes.
FloorPtr CreateFloor(FloorLevel level)
{
	FloorPtr aux = (FloorPtr)malloc(sizeof(Floor));
	aux->level = level;
	aux->roomList = NULL;
	aux->next = NULL;
	return aux;
}
//Adds a Floor to a FloorList.
FloorPtr AddFloorToList(FloorPtr head, FloorPtr node)
{
	FloorPtr aux = head;
	FloorPtr aux2 = aux;
	if (head == NULL)
		head = node;
	else
	{
		while (aux)
		{
			aux2 = aux;
			aux = aux->next;
		}
		aux2->next = node;
		node->next = aux;
	}
	return head;
}
//Removes a Floor from a FloorList.
FloorPtr RemoveFloorFromList(FloorPtr head, FloorLevel level)
{
	FloorPtr aux = head;
	FloorPtr aux2 = aux;
	if (head != NULL)
	{
		while (aux)
		{
			if (aux->level == level)
			{
				aux2->next = aux->next;
				free(aux);
				break;
			}
			aux2 = aux;
			aux = aux->next;
		}
	}
	return head;
}
//Destroy a Floor from memory.
FloorPtr DestroyFloor(FloorPtr floor)
{
	floor->roomList = DestroyRoomList(floor->roomList);
	free(floor);
	return NULL;
}
//Destroy FloorList from memory.
FloorPtr DestroyFloorList(FloorPtr head)
{
	FloorPtr aux = head;
	FloorPtr aux2 = aux;
	while (aux)
	{
		aux = aux->next;
		aux2 = DestroyFloor(aux2);
		aux2 = aux;
	}
	return NULL;
}
#pragma endregion


//Allocates memory for a Map and gives value to its atributes.
MapPtr CreateMap()
{
	MapPtr aux = (MapPtr)malloc(sizeof(Map));
	FloorPtr basement = CreateFloor(BASEMENT);
	FloorPtr ground = CreateFloor(GROUND);
	FloorPtr upper = CreateFloor(UPPER);
	aux->mapFloor = NULL;
	aux->roomCounter = 0;


	aux->mapFloor = AddFloorToList(aux->mapFloor, basement);
	aux->mapFloor = AddFloorToList(aux->mapFloor, ground);
	aux->mapFloor = AddFloorToList(aux->mapFloor, upper);

	return aux;
}
//Releases memory of a Map.
boolean DestroyMap(MapPtr map)
{
	map->mapFloor = DestroyFloorList(map->mapFloor);
	return TRUE;
}

//Allocates memory for a Minion and gives value to its atributes.
MinionPtr CreateMinion(stringPtr name, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
{
	MinionPtr aux = (MinionPtr)malloc(sizeof(Minion));
	if (name != NULL)
		strcpy(aux->name, strupr(name));
	else
		strcpy(aux->name, "NULL");
	aux->might_mod = might_mod;
	aux->speed_mod = speed_mod;
	aux->sanity_mod = sanity_mod;
	aux->intellect_mod = inteligence_mod;
	aux->next = NULL;
	return aux;
}
//Adds a Minion to a MinionList.
MinionPtr DestroyMinion(MinionPtr node)
{
	free(node);
	return NULL;
}
//Assigns a Minion to a player and applies stat changes.
CharacterPtr AssignMinion(CharacterPtr player, MinionPtr minion)
{
	MinionPtr aux = player->minions;

	if (player->minions == NULL)
		player->minions = minion;
	else
	{
		while (aux->next != NULL)
		{
			aux = aux->next;
		}
		aux->next = minion;
	}

	player->might += minion->might_mod;
	player->speed += minion->speed_mod;
	player->sanity += minion->sanity_mod;
	player->inteligence += minion->sanity_mod;

	return player;
}
//Unassign a Minion to a player and removes the stat changes applied previously by that Minion.
boolean UnassignMinion(CharacterPtr player, MinionPtr minion)
{
	MinionPtr aux = player->minions;
	MinionPtr aux2 = aux;

	if (player->minions == NULL)
		return FALSE;
	else
	{
		while (aux != NULL && aux != minion)
		{
			aux2 = aux;
			aux = aux->next;
		}

		if (aux == minion)
		{
			player->might -= minion->might_mod;
			player->speed -= minion->speed_mod;
			player->sanity -= minion->sanity_mod;
			player->inteligence -= minion->sanity_mod;

			aux2->next = aux->next;
			free(aux);
			return TRUE;
		}
		else
			return FALSE;
	}
}
//Add a Minion to the MinionArray.
boolean AddMinionToArray(MasterPtr master, MinionPtr node)
{
	master->cards.minionList[master->cards.minionCount++] = *node;
	return TRUE;
}
//Removes a Minion from the MinionArray.
boolean RemoveMinionFromArray(MasterPtr master, stringPtr node)
{
	MinionPtr aux = CreateMinion(NULL, 0, 0, 0, 0);
	for (int i = 0; i > master->cards.minionCount; i++)
		if (strcmp(master->cards.minionList[i].name, node) == 0)
		{
			for (int j = i; j > master->cards.minionCount - 1; i++)
				master->cards.minionList[j] = master->cards.minionList[j + 1];
			master->cards.minionList[master->cards.minionCount] = *aux;
			return TRUE;
		}
	return FALSE;
}


#pragma region Cards
//Allocates memory for an Event and gives value to its atributes.
EventPtr CreateEvent(stringPtr name, stringPtr description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
{
	EventPtr aux = (EventPtr)malloc(sizeof(Event));

	if (name != NULL)
		strcpy(aux->name, strupr(name));
	else
		strcpy(aux->name, "NULL");
	if (description != NULL)
		strcpy(aux->description, strupr(description));
	else
		strcpy(aux->description, "NULL");
	aux->might_mod = might_mod;
	aux->speed_mod = speed_mod;
	aux->sanity_mod = sanity_mod;
	aux->intellect_mod = inteligence_mod;
	aux->next = NULL;

	return aux;
}
//Adds an Event to an EventList.
EventPtr AddEventToList(EventPtr head, EventPtr node)
{
	EventPtr aux = head;
	EventPtr aux2 = aux;
	if (head == NULL)
		head = node;
	else
	{
		while (aux && strcmp(aux->name, node->name) > 0)
		{
			aux2 = aux;
			aux = aux->next;
		}
		aux2->next = node;
		node->next = aux;
	}
	return head;
}
//Add a Event to the EventArray.
boolean AddEventToArray(MasterPtr master, EventPtr node)
{
	master->cards.eventList[master->cards.eventCount++] = *node;
	return TRUE;
}
//Removes a Event from the EventArray.
boolean RemoveEventFromArray(MasterPtr master, stringPtr node)
{
	EventPtr aux = CreateEvent(NULL, NULL, 0, 0, 0, 0);
	for (int i = 0; i > master->cards.eventCount; i++)
		if (strcmp(master->cards.eventList[i].name, node) == 0)
		{
			for (int j = i; j > master->cards.eventCount - 1; i++)
				master->cards.eventList[j] = master->cards.eventList[j + 1];
			master->cards.eventList[master->cards.eventCount] = *aux;
			return TRUE;
		}
	return FALSE;
}
//Removes an Event from an EventList.
EventPtr RemoveEventFromList(EventPtr head, stringPtr name)
{
	EventPtr aux = head;
	EventPtr aux2 = aux;
	if (head != NULL)
	{
		while (aux)
		{
			if (strcmp(aux->name, name) != 0)
			{
				aux2->next = aux->next;
				free(aux);
				break;
			}
			aux2 = aux;
			aux = aux->next;
		}
	}
	return head;
}
//Destroy an Event from memory
EventPtr DestroyEvent(EventPtr node)
{
	free(node);
	return NULL;
}
//Destroy EventList from memory.
EventPtr DestroyEventList(EventPtr head)
{
	EventPtr aux = head;
	EventPtr aux2 = aux;
	while (aux)
	{
		aux = aux->next;
		aux2 = DestroyEvent(aux2);
		aux2 = aux;
	}
	return NULL;
}

//Allocates memory for an Omen and gives value to its atributes.
OmenPtr CreateOmen(stringPtr name, stringPtr description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
{
	OmenPtr aux = (OmenPtr)malloc(sizeof(Omen));

	if (name != NULL)
		strcpy(aux->name, strupr(name));
	else
		strcpy(aux->name, "NULL");
	if (description != NULL)
		strcpy(aux->description, strupr(description));
	else
		strcpy(aux->description, "NULL");
	aux->might_mod = might_mod;
	aux->speed_mod = speed_mod;
	aux->sanity_mod = sanity_mod;
	aux->intellect_mod = inteligence_mod;
	aux->next = NULL;

	return aux;
}
//Adds an Omen to an OmenList.
OmenPtr AddOmenToList(OmenPtr head, OmenPtr node)
{
	OmenPtr aux = head;
	OmenPtr aux2 = aux;
	if (head == NULL)
		head = node;
	else
	{
		while (aux && strcmp(aux->name, node->name) > 0)
		{
			aux2 = aux;
			aux = aux->next;
		}
		aux2->next = node;
		node->next = aux;
	}
	return head;
}
//Add a Omen to the OmenArray.
boolean AddOmenToArray(MasterPtr master, OmenPtr node)
{
	master->cards.omenList[master->cards.omenCount++] = *node;
	return TRUE;
}
//Removes a Omen from the OmenArray.
boolean RemoveOmentFromArray(MasterPtr master, stringPtr node)
{
	OmenPtr aux = CreateOmen(NULL, NULL, 0, 0, 0, 0);
	for (int i = 0; i > master->cards.omenCount; i++)
		if (strcmp(master->cards.omenList[i].name, node) == 0)
		{
			for (int j = i; j > master->cards.omenCount - 1; i++)
				master->cards.omenList[j] = master->cards.omenList[j + 1];
			master->cards.omenList[master->cards.omenCount] = *aux;
			return TRUE;
		}
	return FALSE;
}
//Removes an Omen from an OmenList.
OmenPtr RemoveOmenFromList(OmenPtr head, stringPtr name)
{
	OmenPtr aux = head;
	OmenPtr aux2 = aux;
	if (head != NULL)
	{
		while (aux)
		{
			if (strcmp(aux->name, name) != 0)
			{
				aux2->next = aux->next;
				free(aux);
				break;
			}
			aux2 = aux;
			aux = aux->next;
		}
	}
	return head;
}
//Destroy an Omen from memory
OmenPtr DestroyOmen(OmenPtr node)
{
	free(node);
	return NULL;
}
//Destroy OmenList from memory.
OmenPtr DestroyOmenList(OmenPtr head)
{
	OmenPtr aux = head;
	OmenPtr aux2 = aux;
	while (aux)
	{
		aux = aux->next;
		aux2 = DestroyOmen(aux2);
		aux2 = aux;
	}
	return NULL;
}

//Allocates memory for an Item and gives value to its atributes.
ItemPtr CreateItem(stringPtr name, stringPtr description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
{
	ItemPtr aux = (ItemPtr)malloc(sizeof(Item));

	if (name != NULL)
		strcpy(aux->name, strupr(name));
	else
		strcpy(aux->name, "NULL");
	if (description != NULL)
		strcpy(aux->description, strupr(description));
	else
		strcpy(aux->description, "NULL");
	aux->might_mod = might_mod;
	aux->speed_mod = speed_mod;
	aux->sanity_mod = sanity_mod;
	aux->intellect_mod = inteligence_mod;
	aux->next = NULL;

	return aux;
}
//Adds an Item to an ItemList.
ItemPtr AddItemToList(ItemPtr head, ItemPtr node)
{
	ItemPtr aux = head;
	ItemPtr aux2 = aux;
	if (head == NULL)
		head = node;
	else
	{
		while (aux && strcmp(aux->name, node->name) > 0)
		{
			aux2 = aux;
			aux = aux->next;
		}
		aux2->next = node;
		node->next = aux;
	}
	return head;
}
//Add a Item to the ItemArray.
boolean AddItemToArray(MasterPtr master, ItemPtr node)
{
	master->cards.itemList[master->cards.itemCount++] = *node;
	return TRUE;
}
//Removes a Item from the ItemArray.
boolean RemoveItemFromArray(MasterPtr master, stringPtr node)
{
	ItemPtr aux = CreateItem(NULL, NULL, 0, 0, 0, 0);
	for (int i = 0; i > master->cards.itemCount; i++)
		if (strcmp(master->cards.itemList[i].name, node) == 0)
		{
			for (int j = i; j > master->cards.itemCount - 1; i++)
				master->cards.itemList[j] = master->cards.itemList[j + 1];
			master->cards.itemList[master->cards.itemCount] = *aux;
			return TRUE;
		}
	return FALSE;
}
//Removes an Item from an ItemList.
ItemPtr RemoveItemFromList(ItemPtr head, stringPtr name)
{
	ItemPtr aux = head;
	ItemPtr aux2 = aux;
	if (head != NULL)
	{
		while (aux)
		{
			if (strcmp(aux->name, name) != 0)
			{
				aux2->next = aux->next;
				free(aux);
				break;
			}
			aux2 = aux;
			aux = aux->next;
		}
	}
	return head;
}
//Destroy an Item from memory
ItemPtr DestroyItem(ItemPtr node)
{
	free(node);
	return NULL;
}
//Assigns an item to a player and applies stat changes.
CharacterPtr AssignItem(CharacterPtr player, ItemPtr item)
{
	ItemPtr aux = player->items;

	if (player->items == NULL)
		player->items = item;
	else
	{
		while (aux->next != NULL)
		{
			aux = aux->next;
		}
		aux->next = item;
	}

	player->might += item->might_mod;
	player->speed += item->speed_mod;
	player->sanity += item->sanity_mod;
	player->inteligence += item->sanity_mod;

	return player;
}
//Unassign an item to a player and removes the stat changes applied previously by that item.
boolean UnassignItem(CharacterPtr player, ItemPtr item)
{
	ItemPtr aux = player->items;
	ItemPtr aux2 = aux;

	if (player->items == NULL)
		return FALSE;
	else
	{
		while (aux != NULL && aux != item)
		{
			aux2 = aux;
			aux = aux->next;
		}

		if (aux == item)
		{
			player->might -= item->might_mod;
			player->speed -= item->speed_mod;
			player->sanity -= item->sanity_mod;
			player->inteligence -= item->sanity_mod;

			aux2->next = aux->next;
			free(aux);
			return TRUE;
		}
		else
			return FALSE;
	}
}
//Destroy ItemList from memory.
ItemPtr DestroyItemList(ItemPtr head)
{
	ItemPtr aux = head;
	ItemPtr aux2 = aux;
	while (aux)
	{
		aux = aux->next;
		aux2 = DestroyItem(aux2);
		aux2 = aux;
	}
	return NULL;
}

//Allocates memory for a Character and gives value to its atributes.
CharacterPtr CreateChar(stringPtr name, int might, int speed, int sanity, int inteligence)
{
	CharacterPtr aux = (CharacterPtr)malloc(sizeof(Character));

	aux->position.x = 0;
	aux->position.y = 0;
	if (name != NULL)
		strcpy(aux->name, strupr(name));
	else
		strcpy(aux->name, "NULL");
	aux->might = might;
	aux->speed = speed;
	aux->sanity = sanity;
	aux->inteligence = inteligence;
	aux->history = NULL;
	aux->minions = NULL;
	aux->items = NULL;
	aux->room = NULL;
	aux->next = NULL;

	return aux;
}
//Adds a Character to a CharacterList.
CharacterPtr AddCharToList(CharacterPtr head, CharacterPtr node)
{
	CharacterPtr aux = head;
	CharacterPtr aux2 = aux;
	if (head == NULL)
		head = node;
	else
	{
		while (aux && strcmp(aux->name, node->name) > 0)
		{
			aux2 = aux;
			aux = aux->next;
		}
		aux2->next = node;
		node->next = aux;
	}
	return head;
}
//Add a Character to the CharacterArray.
boolean AddCharToArray(MasterPtr master, CharacterPtr node)
{
	master->cards.characterList[master->cards.charCount++] = *node;
	return TRUE;
}
//Removes a Character from the CharacterArray.
boolean RemovecharFromArray(MasterPtr master, stringPtr node)
{
	CharacterPtr aux = CreateChar(NULL, 0, 0, 0, 0);
	for (int i = 0; i > master->cards.charCount; i++)
		if (strcmp(master->cards.characterList[i].name, node) == 0)
		{
			for (int j = i; j > master->cards.charCount - 1; i++)
				master->cards.characterList[j] = master->cards.characterList[j + 1];
			master->cards.characterList[master->cards.charCount] = *aux;
			return TRUE;
		}
	return FALSE;
}
//Removes a Character from a CharacterList.
CharacterPtr RemoveCharFromList(CharacterPtr head, stringPtr name)
{
	CharacterPtr aux = head;
	CharacterPtr aux2 = aux;
	if (head != NULL)
	{
		while (aux)
		{
			if (strcmp(aux->name, name) != 0)
			{
				aux2->next = aux->next;
				free(aux);
				break;
			}
			aux2 = aux;
			aux = aux->next;
		}
	}
	return head;
}
//Destroys a Character from memory;
CharacterPtr DestroyChar(CharacterPtr node)
{
	HistoryPtr auxHis1 = node->history;
	HistoryPtr auxHis2 = auxHis1;
	while (auxHis1)
	{
		auxHis1 = auxHis1->next;
		DestroyHistory(auxHis2);
		auxHis2 = auxHis1;
	}

	ItemPtr auxItem1 = node->items;
	ItemPtr auxItem2 = auxItem1;
	while (auxHis1)
	{
		auxItem1 = auxItem1->next;
		DestroyHistory(auxItem2);
		auxItem2 = auxItem1;
	}

	MinionPtr auxMin1 = node->minions;
	MinionPtr auxMin2 = auxMin1;
	while (auxMin1)
	{
		auxMin1 = auxMin1->next;
		UnassignMinion(node, auxMin2);
		auxMin2 = auxMin1;
	}

	node->room = DestroyRoomList(node->room);

	free(node);
	return NULL;
}
//Destroy CharList from memory.
CharacterPtr DestroyCharList(CharacterPtr head)
{
	CharacterPtr aux = head;
	CharacterPtr aux2 = aux;
	while (aux)
	{
		aux = aux->next;
		aux2 = DestroyChar(aux2);
		aux2 = aux;
	}
	return NULL;
}
#pragma endregion

//Gets a random number off a given stat
unsigned int DiceRoll(int stat)
{
	int rv = 0;

	for (int i = 1; i <= stat; i++) //we roll the dice as many times as the stat given. ex: a might roll of 3 is rolling 3 dice.
	{
		rv += rand() % 3; //since the die in this game has 2 zeros, 2 ones and 2 twos, we just random as if it only had 3 sides.
	}

	return rv;
}

#pragma region Auxiliar functions

void AddCharacterCards(Character infoChar[])
{
	string name;
	int might, speed, sanity, inteligence;

	FILE *f = fopen("cards.bin", "ab");

	for (int i = 0; i < MAX_CHARACTERS; i++)
	{
		printf("Insert characters name: ");
		scanf("%s", name);
		printf("\n");
		printf("Might: ");
		scanf("%d", might);
		printf("\n");
		printf("Speed: ");
		scanf("%d", speed);
		printf("\n");
		printf("Sanity: ");
		scanf("%d", sanity);
		printf("\n");
		printf("Inteligence: ");
		scanf("%d", inteligence);
		printf("\n");

		CharacterPtr newChar = CreateChar(name, might, speed, sanity, inteligence);
		infoChar[i] = *newChar;


		fwrite(newChar, sizeof(Character), 1, f);
	}

	fclose(f);
}
void AddEventCards(Event infoEvent[])
{
	string name, description;
	int might_mod, speed_mod, sanity_mod, inteligence_mod;

	FILE *f = fopen("cards.bin", "ab");

	for (int i = 0; i < MAX_EVENTS; i++)
	{
		printf("Insert event name: ");
		scanf("%s", name);
		printf("\n");
		printf("Insert the event's description: ");
		scanf("%s", description);
		printf("\n");
		printf("Might: ");
		scanf("%d", might_mod);
		printf("\n");
		printf("Speed: ");
		scanf("%d", speed_mod);
		printf("\n");
		printf("Sanity: ");
		scanf("%d", sanity_mod);
		printf("\n");
		printf("Inteligence: ");
		scanf("%d", inteligence_mod);
		printf("\n");

		EventPtr newEvent = CreateEvent(name, description, might_mod, speed_mod, sanity_mod, inteligence_mod);
		infoEvent[i] = *newEvent;

		fwrite(newEvent, sizeof(Event), 1, f);
	}

	fclose(f);
}
void AddOmenCards(Omen infoOmen[])
{
	string name, description;
	int might_mod, speed_mod, sanity_mod, inteligence_mod;

	FILE *f = fopen("cards.bin", "ab");

	for (int i = 0; i < MAX_OMENS; i++)
	{
		printf("Insert omen name: ");
		scanf("%s", name);
		printf("\n");
		printf("Insert the omen's description: ");
		scanf("%s", description);
		printf("\n");
		printf("Might: ");
		scanf("%d", might_mod);
		printf("\n");
		printf("Speed: ");
		scanf("%d", speed_mod);
		printf("\n");
		printf("Sanity: ");
		scanf("%d", sanity_mod);
		printf("\n");
		printf("Inteligence: ");
		scanf("%d", inteligence_mod);
		printf("\n");

		OmenPtr newOmen = CreateOmen(name, description, might_mod, speed_mod, sanity_mod, inteligence_mod);
		infoOmen[i] = *newOmen;

		fwrite(newOmen, sizeof(Omen), 1, f);

	}

	fclose(f);
}
void AddItemCards(Item infoItem[])
{
	string name, description;
	int might_mod, speed_mod, sanity_mod, inteligence_mod;

	FILE *f = fopen("cards.bin", "ab");

	for (int i = 0; i < MAX_ITEMS; i++)
	{
		printf("Insert item name: ");
		scanf("%s", name);
		printf("\n");
		printf("Insert the card's description: ");
		scanf("%s", description);
		printf("\n");
		printf("Might: ");
		scanf("%d", might_mod);
		printf("\n");
		printf("Speed: ");
		scanf("%d", speed_mod);
		printf("\n");
		printf("Sanity: ");
		scanf("%d", sanity_mod);
		printf("\n");
		printf("Inteligence: ");
		scanf("%d", inteligence_mod);
		printf("\n");

		ItemPtr newItem = CreateItem(name, description, might_mod, speed_mod, sanity_mod, inteligence_mod);
		infoItem[i] = *newItem;

		fwrite(newItem, sizeof(Item), 1, f);
	}
	fclose(f);
}
//Uses the functions above to ask for card information and add it to the database.
void AddCards(CardPtr c)
{
	AddCharacterCards(c->characterList);
	AddEventCards(c->eventList);
	AddOmenCards(c->omenList);
	AddItemCards(c->itemList);

	printf("CARDS SAVED...");
}
void Reset(MasterPtr master)
{
	master->characterList = DestroyCharList(master->characterList);
	CharacterPtr auxchar = CreateChar(NULL, 0, 0, 0, 0);
	EventPtr auxevent = CreateEvent(NULL, NULL, 0, 0, 0, 0);
	ItemPtr auxitem = CreateItem(NULL, NULL, 0, 0, 0, 0);
	MinionPtr auxmin = CreateMinion(NULL, 0, 0, 0, 0);
	OmenPtr auxomen = CreateOmen(NULL, NULL, 0, 0, 0, 0);
	RoomPtr auxrooom = CreateRoom(NULL, NULL, NULL, EMPTY, EMPTY, EMPTY, EMPTY);

	master->cards.charCount = 0;
	master->cards.eventCount = 0;
	master->cards.itemCount = 0;
	master->cards.minionCount = 0;
	master->cards.omenCount = 0;
	master->cards.roomCount = 0;

	for (int i = 0; i < MAX_CHARACTERS; i++)
		master->cards.characterList[i] = *auxchar;
	for (int i = 0; i < MAX_EVENTS; i++)
		master->cards.eventList[i] = *auxevent;
	for (int i = 0; i < MAX_ITEMS; i++)
		master->cards.itemList[i] = *auxitem;
	for (int i = 0; i < MAX_MINIONS; i++)
		master->cards.minionList[i] = *auxmin;
	for (int i = 0; i < MAX_OMENS; i++)
		master->cards.omenList[i] = *auxomen;
	for (int i = 0; i < MAX_ROOMS; i++)
		master->cards.roomList[i] = *auxrooom;

	auxchar = DestroyChar(auxchar);
	auxevent = DestroyEvent(auxevent);
	auxitem = DestroyItem(auxitem);
	auxmin = DestroyMinion(auxmin);
	auxomen = DestroyOmen(auxomen);
	auxrooom = DestroyRoom(auxrooom);
}
#pragma endregion
//Writes to the "cards.bin" file all the cards taht compose the game.
boolean WriteCards(CardPtr c)
{
	FILE *f = fopen("cards.bin", "wb");
	if (f)
	{
		fwrite(c->characterList, sizeof(Character), MAX_CHARACTERS, f);
		fwrite(c->eventList, sizeof(Event), MAX_EVENTS, f);
		fwrite(c->omenList, sizeof(Omen), MAX_OMENS, f);
		fwrite(c->itemList, sizeof(Item), MAX_ITEMS, f);
		fwrite(c->minionList, sizeof(Minion), MAX_MINIONS, f);
		fwrite(c->roomList, sizeof(Room), MAX_ROOMS, f);

		fwrite(c->charCount, sizeof(int), 1, f);
		fwrite(c->eventCount, sizeof(int), 1, f);
		fwrite(c->omenCount, sizeof(int), 1, f);
		fwrite(c->itemCount, sizeof(int), 1, f);
		fwrite(c->minionCount, sizeof(int), 1, f);
		fwrite(c->roomCount, sizeof(int), 1, f);
		fclose(f);
		return TRUE;
	}
	return FALSE;
}
//Reads from the "cards.bin" file all the cards that compose the game.
boolean LoadCards(CardPtr c)
{
	FILE *f = fopen("cards.bin", "rb");
	if (f)
	{
		fread(c->characterList, sizeof(Character), MAX_CHARACTERS, f);
		fread(c->eventList, sizeof(Event), MAX_EVENTS, f);
		fread(c->omenList, sizeof(Omen), MAX_OMENS, f);
		fread(c->itemList, sizeof(Item), MAX_ITEMS, f);
		fread(c->minionList, sizeof(Minion), MAX_MINIONS, f);
		fread(c->roomList, sizeof(Room), MAX_ROOMS, f);

		fread(c->charCount, sizeof(int), 1, f);
		fread(c->eventCount, sizeof(int), 1, f);
		fread(c->omenCount, sizeof(int), 1, f);
		fread(c->itemCount, sizeof(int), 1, f);
		fread(c->minionCount, sizeof(int), 1, f);
		fread(c->roomCount, sizeof(int), 1, f);

		fclose(f);
		return TRUE;
	}
	return FALSE;
}
//Loads the master item.
boolean LoadMaster(MasterPtr master)
{
	master->omenTrack = 0;
	master->characterList = NULL;
	boolean cards = LoadCards(&(master->cards));
	master->map = *CreateMap();

	if (cards == TRUE)
		return TRUE;
	return FALSE;
}
//Unloads the master item.
boolean EndMaster(MasterPtr master)
{
	boolean cards = WriteCards(&(master->cards));
	boolean map = DestroyMap(&(master->map));
	//master->characterList = DestroyCharList(master->characterList);

	if (cards == TRUE && map == TRUE)
		return TRUE;
	return FALSE;
}