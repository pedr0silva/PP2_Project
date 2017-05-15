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

//Allocates memory for database cards.
CardPtr CreateDatabase(void)
{
	CardPtr aux = (CardPtr)malloc(sizeof(Card));

	return aux;
}

#pragma region Rooms
//Assigns values to all four walls of a room.
boolean AssignWalls(RoomWallPtr arr, WallType upType, WallType leftType, WallType downType, WallType rightType)
{
	RoomWall aux;
	aux.Direction = Up;
	aux.WallType = upType;
	arr[0] = aux;

	aux.Direction = Left;
	aux.WallType = leftType;
	arr[1] = aux;

	aux.Direction = Down;
	aux.WallType = downType;
	arr[2] = aux;

	aux.Direction = Right;
	aux.WallType = rightType;
	arr[3] = aux;

	return TRUE;
}
//Returns the wall of that direction in that room.
RoomWallPtr FindWallDirection(RoomPtr room, Direction direction)
{
	RoomWallPtr aux = NULL;
	if (direction == Up)
		aux = room->wall;
	else if (direction == Left)
		aux = &(room->wall[1]);
	else if (direction == Down)
		aux = &(room->wall[2]);
	else if (direction == Right)
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
RoomPtr CreateRoom(string roomName, EventPtr roomEvent, OmenPtr roomOmen, WallType upType, WallType leftType, WallType downType, WallType rightType)
{
	RoomPtr aux = (RoomPtr)malloc(sizeof(Room));

	if (aux != NULL)
	{
		aux->position.x = 0;
		aux->position.y = 0;
		aux->position.z = 0;
		aux->positionLenght = 0;
		aux->name = roomName;
		aux->event = roomEvent;
		aux->omen = roomOmen;
		aux->next = NULL;
		boolean i = AssignWalls(&(aux->wall), upType, leftType, downType, rightType);
	}

	return aux;
}
//Creates a copy of a room to use in a game.
RoomPtr InstanciateRoom(RoomPtr room, Vector3 position)
{
	RoomPtr aux = (RoomPtr)malloc(sizeof(Room));
	aux->event = room->event;
	aux->name = room->name;
	aux->omen = room->omen;
	aux->position = position;
	aux->positionLenght = aux->position.x + aux->position.y;
	aux->next = NULL;
	boolean i = CopyWalls(aux->wall, room->wall);

	if (i == TRUE)
		return aux;
	return NULL;
}
//Adds a Room to a RoomList.
RoomPtr AddRoomToList(RoomPtr head, RoomPtr node)
{
	RoomPtr aux = head;
	RoomPtr aux2 = aux;


	if (head == NULL)
		head == node;
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
//Opens a room and generates ones.
boolean OpenRoom(FloorPtr floor, RoomPtr currentRoom, Direction direction)
{
	RoomWallPtr aux = FindWallDirection(currentRoom, direction);
	RoomPtr roomAux = NULL;

	if (aux->WallType == Door)
	{
		Vector3 addingValue = currentRoom->position;
		if (direction == Up)
			addingValue.y++;
		else if (direction == Left)
			addingValue.x--;
		else if (direction == Down)
			addingValue.y--;
		else if (direction == Right)
			addingValue.x++;
		//needs code here to generate a room.
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
		head == node;
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
#pragma endregion

//Allocates memory for a Map and gives value to its atributes.
MapPtr CreateMap()
{
	MapPtr aux = (MapPtr)malloc(sizeof(Map));
	FloorPtr basement = CreateFloor(BASEMENT);
	FloorPtr ground = CreateFloor(GROUND);
	FloorPtr upper = CreateFloor(UPPER);

	aux->mapFloor = AddFloorToList(aux->mapFloor, basement);
	aux->mapFloor = AddFloorToList(aux->mapFloor, ground);
	aux->mapFloor = AddFloorToList(aux->mapFloor, upper);
	aux->roomCounter = 0;

	return aux;
}
boolean IniticializeMap(MapPtr map)
{
	FloorPtr basement = CreateFloor(BASEMENT);
	FloorPtr ground = CreateFloor(GROUND);
	FloorPtr upper = CreateFloor(UPPER);

	map->mapFloor = AddFloorToList(map->mapFloor, basement);
	map->mapFloor = AddFloorToList(map->mapFloor, ground);
	map->mapFloor = AddFloorToList(map->mapFloor, upper);
	map->roomCounter = 0;

	return TRUE;
}

#pragma region Cards
//Allocates memory for a Character and gives value to its atributes.
CharacterPtr CreateChar(string name, int might, int speed, int sanity, int inteligence)
{
	CharacterPtr aux = (CharacterPtr)malloc(sizeof(Character));

	aux->position.x = 0;
	aux->position.y = 0;
	aux->position.z = 0;
	aux->name = ToUpper(name);
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
		head == node;
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

//Allocates memory for an Event and gives value to its atributes.
EventPtr CreateEvent(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
{
	EventPtr aux = (EventPtr)malloc(sizeof(Event));

	aux->name = ToUpper(name);
	aux->description = ToUpper(description);
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
		head == node;
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

//Allocates memory for an Omen and gives value to its atributes.
OmenPtr CreateOmen(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
{
	OmenPtr aux = (OmenPtr)malloc(sizeof(Omen));

	aux->name = ToUpper(name);
	aux->description = ToUpper(description);
	aux->might_mod = might_mod;
	aux->speed_mod = speed_mod;
	aux->sanity_mod = sanity_mod;
	aux->intellect_mod = inteligence_mod;
	aux->next = NULL;

	return aux;
}//Adds an Event to an EventList.
 //Adds an Omen to an OmenList.
OmenPtr AddOmenToList(OmenPtr head, OmenPtr node)
{
	OmenPtr aux = head;
	OmenPtr aux2 = aux;
	if (head == NULL)
		head == node;
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

//Allocates memory for an Item and gives value to its atributes.
ItemPtr CreateItem(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
{
	ItemPtr aux = (ItemPtr)malloc(sizeof(Item));

	aux->name = ToUpper(name);
	aux->description = ToUpper(description);
	aux->might_mod = might_mod;
	aux->speed_mod = speed_mod;
	aux->sanity_mod = sanity_mod;
	aux->intellect_mod = inteligence_mod;
	aux->next = NULL;

	return aux;
}
//Adds an Item to an ItemList.
ItemPtr AddItemToList(ItemPtr head, OmenPtr node)
{
	ItemPtr aux = head;
	ItemPtr aux2 = aux;
	if (head == NULL)
		head == node;
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

//Assigns a minion to a player and applies stat changes.
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
//Unassign a minion to a player and removes the stat changes applied previously by that minion.
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
		fclose(f);
		return TRUE;
	}
	return FALSE;
}

#pragma endregion

MasterPtr LoadMaster(MasterPtr master)
{
	master->damageTrack = 0;
	boolean cards = LoadCards(&(master->Cards));
	return master;
}