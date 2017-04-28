/*
File: functions.c
Description: This file has the needed functions to operate the data structures.
/!\ we may want to create various function files. ex: creator.c, listmanager.c, etc...
------------//---------------//------------//---------------//------------//---------------
Name: Pedro Silva
Email: pmiguelfs@gmail.com
Date Log:
14/04/2017 - Created a few basic functions to create maps, rooms and characters
27/04/2017 - Worked on auxiliar functions to insert/modify the cards available in the game.
28/04/2017 - Added DiceRoll, Asign and Unasign Minion and Item functions
------------//---------------//------------//---------------//------------//---------------
Name: Diogo Portela
Email:
Date Log:

*/

#include <stdio.h>
#include "estructs.h"

//Allocates memory for a Map and gives value to it's atributes.
MapPtr CreateMap(Floor mapFloor)
{
	MapPtr aux = (MapPtr)malloc(sizeof(Map));

	aux->mapFloor = mapFloor;
	aux->roomList = NULL;
	aux->roomCounter = 0;
	aux->next = NULL;
	aux->prev = NULL;

	return aux;
}

//Allocates memory for a Room and gives value to it's atributes.
/*RoomPtr CreateRoom(string roomName, EventPtr roomEvent, OmenPtr roomOmen)
{
	RoomPtr aux = (RoomPtr)malloc(sizeof(Room));

	aux->name = roomName;
	aux->event = roomEvent;
	aux->omen = roomOmen;
	aux->next = NULL;

	return aux;
}*/

//Allocates memory for a Character and gives value to it's atributes.
CharacterPtr CreateChar(string name, int might, int speed, int sanity, int inteligence)
{
	CharacterPtr aux = (CharacterPtr)malloc(sizeof(Character));

	aux->name = name;
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

//Allocates memory for an Event and gives value to it's atributes.
EventPtr CreateEvent(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
{
	EventPtr aux = (EventPtr)malloc(sizeof(Event));

	aux->name = name;
	aux->description = description;
	aux->might_mod = might_mod;
	aux->speed_mod = speed_mod;
	aux->sanity_mod = sanity_mod;
	aux->intellect_mod = inteligence_mod;
	aux->next = NULL;

	return aux;
}

//Allocates memory for an Omen and gives value to it's atributes.
OmenPtr CreateOmen(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
{
	OmenPtr aux = (OmenPtr)malloc(sizeof(Omen));

	aux->name = name;
	aux->description = description;
	aux->might_mod = might_mod;
	aux->speed_mod = speed_mod;
	aux->sanity_mod = sanity_mod;
	aux->intellect_mod = inteligence_mod;
	aux->next = NULL;

	return aux;
}

//Allocates memory for an Item and gives value to it's atributes.
ItemPtr CreateItem(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod)
{
	ItemPtr aux = (ItemPtr)malloc(sizeof(Item));

	aux->name = name;
	aux->description = description;
	aux->might_mod = might_mod;
	aux->speed_mod = speed_mod;
	aux->sanity_mod = sanity_mod;
	aux->intellect_mod = inteligence_mod;
	aux->next = NULL;

	return aux;
}

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

//Asigns a minion to a player and applies stat changes
CharacterPtr AsignMinion(CharacterPtr player, MinionPtr minion)
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

//Unasign a minion to a player and removes the stat changes applied previously by that minion
boolean UnasignMinion(CharacterPtr player, MinionPtr minion)
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

//Asigns an item to a player and applies stat changes
CharacterPtr AsignItem(CharacterPtr player, ItemPtr item)
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

//Unasign an item to a player and removes the stat changes applied previously by that item
boolean UnasignItem(CharacterPtr player, ItemPtr item)
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

//Uses the functions above to ask for card information and add it to the database
void AddCards(Card c)
{
	AddCharacterCards(c.characterList);
	AddEventCards(c.eventList);
	AddOmenCards(c.omenList);
	AddItemCards(c.itemList);

	printf("CARDS SAVED...");

}

#pragma endregion