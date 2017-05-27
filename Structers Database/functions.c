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

#include "Structs.h"
#include "dirent.h"

#pragma region Auxiliary Structs Functions
	//Allocates memory for a Vector2 and gives value to its atributes.
	Vector2Ptr CreateVector2(int x, int y)
	{
		Vector2Ptr aux = (Vector2Ptr)malloc(sizeof(Vector2));
		aux->x = x;
		aux->y = y;
		return aux;
	}
	//Change the values of a Vector2.
	Vector2 ChangeVector2(int x, int y)
	{
		Vector2 aux;
		aux.x = x;
		aux.y = y;
		return aux;
	}
	//Destroy Vector2 from memory.
	Vector2Ptr DestroyVector2(Vector2Ptr node)
	{
		free(node);
		return NULL;
	}

	//Initializes an empty String.
	void InitString(string string)
	{
		for (int i = 0; i < MAX_STRING; i++)
		{
			string[i] = 0;
		}
	}

	//Allocates memory for a History and gives value to its atributes.
	HistoryPtr CreateHistory(string text)
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
#pragma endregion

#pragma region Rooms
	//Assigns values to all four walls of a room.
	BOOL AssignWalls(RoomWallPtr arr, WallType upType, WallType leftType, WallType downType, WallType rightType)
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
	BOOL CopyWalls(RoomWallPtr thisRoomWalls, RoomWallPtr otherRoomWalls)
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
			AssignWalls(room->wall, room->wall[1].WallType, room->wall[2].WallType, room->wall[3].WallType, room->wall[0].WallType);
		if (value < 0)
			AssignWalls(room->wall, room->wall[4].WallType, room->wall[0].WallType, room->wall[1].WallType, room->wall[2].WallType);
		return room;
	}

	//Allocates memory for a Room and gives value to its atributes.
	RoomPtr CreateRoom(string roomName, EventPtr roomEvent, OmenPtr roomOmen, WallType upType, WallType leftType, WallType downType, WallType rightType)
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
			BOOL i = AssignWalls(&(aux->wall), upType, leftType, downType, rightType);
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
		BOOL i = CopyWalls(aux->wall, room->wall);

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
	BOOL AddRoomToArray(MasterPtr master, RoomPtr node)
	{
		if (master->cards.roomCount < MAX_ROOMS)
		{
			master->cards.roomList[master->cards.roomCount++] = *node;
			return TRUE;
		}
		return FALSE;
	}
	//Removes a Room from the RoomArray.
	BOOL RemoveRoomFromArray(MasterPtr master, string node)
	{
		RoomPtr aux = CreateRoom(NULL, NULL, NULL, EMPTY, EMPTY, EMPTY, EMPTY);
		for(int i = 0; i < master->cards.roomCount; i++)
			if (strcmp(master->cards.roomList[i].name, node) == 0)
			{
				for (int j = i; j < master->cards.roomCount - 1; j++)
					master->cards.roomList[j] = master->cards.roomList[j + 1];
				master->cards.roomList[--(master->cards.roomCount)] = *aux;
				return TRUE;
			}
		return FALSE;
	}
	//Removes a Room from a RoomList.
	RoomPtr RemoveRoomFromList(RoomPtr head, string name)
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
	//Returns a random Room from the RoomList.
	RoomPtr RandomRoom(MasterPtr master)
	{
		RoomPtr aux = NULL;
		int random = rand() % master->cards.roomCount;
		aux = &(master->cards.roomList[random]);
		return aux;
	}
	//Opens a Room and generates one.
	BOOL OpenRoom(MasterPtr master, FloorPtr floor, RoomPtr currentRoom, Direction direction)
	{
		RoomWallPtr wallAux = FindWallDirection(currentRoom, direction);
		RoomPtr roomAux = NULL;
		RoomPtr roomRandom = NULL;
		Vector2 vectorAux;

		if (wallAux->WallType == DOOR)
		{
			vectorAux = currentRoom->position;
			if (direction == UP)
				vectorAux.y += ROOM_SIZE;
			else if (direction == LEFT)
				vectorAux.x -= ROOM_SIZE;
			else if (direction == DOWN)
				vectorAux.y -= ROOM_SIZE;
			else if (direction == RIGHT)
				vectorAux.x += ROOM_SIZE;;

			roomRandom = RandomRoom(master);

			roomAux = InstanciateRoom(roomRandom, vectorAux);
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

#pragma region Map
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
	BOOL DestroyMap(MapPtr map)
	{
		map->mapFloor = DestroyFloorList(map->mapFloor);
		return TRUE;
	}
#pragma endregion

#pragma region Minion
	//Allocates memory for a Minion and gives value to its atributes.
	MinionPtr CreateMinion(string name, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
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
	//Add a Minion to the MinionArray.
	BOOL AddMinionToArray(MasterPtr master, MinionPtr node)
	{
		if (master->cards.minionCount < MAX_MINIONS)
		{
			master->cards.minionList[master->cards.minionCount++] = *node;
			return TRUE;
		}
		return FALSE;
	}
	//Removes a Minion from the MinionArray.
	BOOL RemoveMinionFromArray(MasterPtr master, string node)
	{
		MinionPtr aux = CreateMinion(NULL, 0, 0, 0, 0);
		for (int i = 0; i < master->cards.minionCount; i++)
			if (strcmp(master->cards.minionList[i].name, node) == 0)
			{
				for (int j = i; j < master->cards.minionCount - 1; j++)
					master->cards.minionList[j] = master->cards.minionList[j + 1];
				master->cards.minionList[--(master->cards.minionCount)] = *aux;
				return TRUE;
			}
		return FALSE;
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
	BOOL UnassignMinion(CharacterPtr player, MinionPtr minion)
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
#pragma endregion

#pragma region Cards

	#pragma region EVENTS
		//Allocates memory for an Event and gives value to its atributes.
		EventPtr Create_Event(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
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
		BOOL AddEventToArray(MasterPtr master, EventPtr node)
		{
			if (master->cards.eventCount < MAX_EVENTS)
			{
				master->cards.eventList[master->cards.eventCount++] = *node;
				return TRUE;
			}
			return FALSE;
		}
		//Removes a Event from the EventArray.
		BOOL RemoveEventFromArray(MasterPtr master, string node)
		{
			EventPtr aux = Create_Event(NULL, NULL, 0, 0, 0, 0);
			for (int i = 0; i < master->cards.eventCount; i++)
				if (strcmp(master->cards.eventList[i].name, node) == 0)
				{
					for (int j = i; j < master->cards.eventCount - 1; j++)
						master->cards.eventList[j] = master->cards.eventList[j + 1];
					master->cards.eventList[--(master->cards.eventCount)] = *aux;
					return TRUE;
				}
			return FALSE;
		}
		//Removes an Event from an EventList.
		EventPtr RemoveEventFromList(EventPtr head, string name)
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
		//Returns a random Event from the EventList.
		EventPtr RandomEvent(MasterPtr master)
		{
			EventPtr aux = NULL;
			int random = rand() % master->cards.eventCount;
			aux = &(master->cards.eventList[random]);
			return aux;
		}
	#pragma endregion

	#pragma region OMEN
		//Allocates memory for an Omen and gives value to its atributes.
		OmenPtr CreateOmen(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
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
		BOOL AddOmenToArray(MasterPtr master, OmenPtr node)
		{
			if (master->cards.omenCount < MAX_OMENS)
			{
				master->cards.omenList[master->cards.omenCount++] = *node;
				return TRUE;
			}
			return FALSE;
			
		}
		//Removes a Omen from the OmenArray.
		BOOL RemoveOmentFromArray(MasterPtr master, string node)
		{
			OmenPtr aux = CreateOmen(NULL, NULL, 0, 0, 0, 0);
			for (int i = 0; i < master->cards.omenCount; i++)
				if (strcmp(master->cards.omenList[i].name, node) == 0)
				{
					for (int j = i; j < master->cards.omenCount - 1; j++)
						master->cards.omenList[j] = master->cards.omenList[j + 1];
					master->cards.omenList[--(master->cards.omenCount)] = *aux;
					return TRUE;
				}
			return FALSE;
		}
		//Removes an Omen from an OmenList.
		OmenPtr RemoveOmenFromList(OmenPtr head, string name)
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
		//Returns a random Omen from the OmenList.
		OmenPtr RandomOmen(MasterPtr master)
		{
			OmenPtr aux = NULL;
			int random = rand() % master->cards.omenCount;
			aux = &(master->cards.omenList[random]);
			return aux;
		}
	#pragma endregion

	#pragma region ITEMS
		//Allocates memory for an Item and gives value to its atributes.
		ItemPtr CreateItem(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
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
		BOOL AddItemToArray(MasterPtr master, ItemPtr node)
		{
			if (master->cards.itemCount < MAX_ITEMS)
			{
				master->cards.itemList[master->cards.itemCount++] = *node;
				return TRUE;
			}
			return FALSE;
		}
		//Removes a Item from the ItemArray.
		BOOL RemoveItemFromArray(MasterPtr master, string node)
		{
			ItemPtr aux = CreateItem(NULL, NULL, 0, 0, 0, 0);
			for (int i = 0; i < master->cards.itemCount; i++)
				if (strcmp(master->cards.itemList[i].name, node) == 0)
				{
					for (int j = i; j < master->cards.itemCount - 1; j++)
						master->cards.itemList[j] = master->cards.itemList[j + 1];
					master->cards.itemList[--(master->cards.itemCount)] = *aux;
					return TRUE;
				}
			return FALSE;
		}
		//Removes an Item from an ItemList.
		ItemPtr RemoveItemFromList(ItemPtr head, string name)
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
		//Returns a random Item from the ItemList.
		ItemPtr RandomItem(MasterPtr master)
		{
			OmenPtr aux = NULL;
			int random = rand() % master->cards.itemCount;
			aux = &(master->cards.itemList[random]);
			return aux;
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
		BOOL UnassignItem(CharacterPtr player, ItemPtr item)
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
	#pragma endregion

	#pragma region CHAR
		//Allocates memory for a Character and gives value to its atributes.
		CharacterPtr CreateChar(string name, int might, int speed, int sanity, int inteligence)
		{
			CharacterPtr aux = (CharacterPtr)malloc(sizeof(Character));

			aux->position.x = 0;
			aux->position.y = 0;
			aux->might = might;
			aux->speed = speed;
			aux->sanity = sanity;
			aux->inteligence = inteligence;
			aux->playerNumber = 0;
			aux->history = NULL;
			aux->minions = NULL;
			aux->items = NULL;
			aux->room = NULL;
			aux->next = NULL;
			if (name != NULL)
				strcpy(aux->name, strupr(name));
			else
				strcpy(aux->name, "NULL");
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
		BOOL AddCharToArray(MasterPtr master, CharacterPtr node)
		{
			if (master->cards.charCount < MAX_CHARACTERS)
			{
				master->cards.characterList[master->cards.charCount++] = *node;
				return TRUE;
			}
			return FALSE;
		}
		//Removes a Character from the CharacterArray.
		BOOL RemoveCharFromArray(MasterPtr master, string node)
		{
			CharacterPtr aux = CreateChar(NULL, 0, 0, 0, 0);
			for (int i = 0; i < master->cards.charCount; i++)
				if (strcmp(master->cards.characterList[i].name, node) == 0)
				{
					for (int j = i; j < master->cards.charCount - 1; j++)
						master->cards.characterList[j] = master->cards.characterList[j + 1];
					master->cards.characterList[--(master->cards.charCount)] = *aux;
					return TRUE;
				}
			return FALSE;
		}
		//Removes a Character from a CharacterList.
		CharacterPtr RemoveCharFromList(CharacterPtr head, string name)
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
		//Returns a random Character from the CharacterList.
		CharacterPtr RandomChar(MasterPtr master)
		{
			CharacterPtr aux = NULL;
			int random = rand() % master->cards.charCount;
			aux = &(master->cards.characterList[random]);
			return aux;
		}
	#pragma endregion

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

#pragma region DataManagement
	//Resests all data of a Master.
	BOOL Reset(MasterPtr master)
	{
		CharacterPtr auxchar = CreateChar(NULL, 0, 0, 0, 0);
		EventPtr auxevent = Create_Event(NULL, NULL, 0, 0, 0, 0);
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
		return TRUE;
	}
	//Writes to the file all the cards that compose the game.
	BOOL WriteCards(CardPtr c, string fileName)
	{
		strcat(fileName, ".bin");

		FILE *f = fopen(fileName, "wb");
		if (f)
		{
			fwrite(c->characterList, sizeof(Character), MAX_CHARACTERS, f);
			fwrite(c->eventList, sizeof(Event), MAX_EVENTS, f);
			fwrite(c->omenList, sizeof(Omen), MAX_OMENS, f);
			fwrite(c->itemList, sizeof(Item), MAX_ITEMS, f);
			fwrite(c->minionList, sizeof(Minion), MAX_MINIONS, f);
			fwrite(c->roomList, sizeof(Room), MAX_ROOMS, f);

			fwrite(&(c->charCount), sizeof(unsigned int), 1, f);
			fwrite(&(c->eventCount), sizeof(unsigned int), 1, f);
			fwrite(&(c->omenCount), sizeof(unsigned int), 1, f);
			fwrite(&(c->itemCount), sizeof(unsigned int), 1, f);
			fwrite(&(c->minionCount), sizeof(unsigned int), 1, f);
			fwrite(&(c->roomCount), sizeof(unsigned int), 1, f);
			fclose(f);
			return TRUE;
		}
		return FALSE;
	}
	//Reads from the file all the cards that compose the game.
	BOOL LoadCards(CardPtr c, string fileName)
	{
		strcat(fileName, ".bin");

		FILE *f = fopen(fileName, "rb");
		if (f)
		{
			fread(c->characterList, sizeof(Character), MAX_CHARACTERS, f);
			fread(c->eventList, sizeof(Event), MAX_EVENTS, f);
			fread(c->omenList, sizeof(Omen), MAX_OMENS, f);
			fread(c->itemList, sizeof(Item), MAX_ITEMS, f);
			fread(c->minionList, sizeof(Minion), MAX_MINIONS, f);
			fread(c->roomList, sizeof(Room), MAX_ROOMS, f);

			fread(&(c->charCount), sizeof(unsigned int), 1, f);
			fread(&(c->eventCount), sizeof(unsigned int), 1, f);
			fread(&(c->omenCount), sizeof(unsigned int), 1, f);
			fread(&(c->itemCount), sizeof(unsigned int), 1, f);
			fread(&(c->minionCount), sizeof(unsigned int), 1, f);
			fread(&(c->roomCount), sizeof(unsigned int), 1, f);

			fclose(f);
			return TRUE;
		}
		return FALSE;
	}
	//Loads the master item.
	BOOL LoadMaster(MasterPtr master, string fileName)
	{
		master->omenTrack = 0;
		master->characterList = NULL;
		BOOL cards = LoadCards(&(master->cards), fileName);
		master->map = *CreateMap();

		if (cards == TRUE)
			return TRUE;
		return FALSE;
	}
	//Unloads the master item.
	BOOL EndMaster(MasterPtr master, string fileName)
	{
		BOOL cards = WriteCards(&(master->cards), fileName);
		BOOL map = DestroyMap(&(master->map));
		master->characterList = DestroyCharList(master->characterList);

		if (cards == TRUE && map == TRUE)
			return TRUE;
		return FALSE;
	}
	//Reads File Directory
	string* ReadSaveDirectory()
	{
		DIR *dir;
		struct dirent *pp;
		string databases[MAX_SAVES];
		int i = 0, length;
		dir = opendir("C:\Users\Pedro\Desktop\dev\PP2_Project\Betrayal\save");

		if (dir != NULL)
		{
			while ((pp = readdir(dir)) != NULL) 
			{
				length = strlen(pp->d_name);
				if (strncmp(pp->d_name + length - 4, ".bin", 4) == 0) 
				{
					*databases[i] = pp->d_name;
					i++;
				}
			}
			closedir(dir);
		}
		return databases;
	}
#pragma endregion