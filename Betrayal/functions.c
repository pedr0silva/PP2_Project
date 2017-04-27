/*
File: functions.c
Description: This file has the needed functions to operate the data structures.
/!\ we may want to create various function files. ex: creator.c, listmanager.c, etc...
------------//---------------//------------//---------------//------------//---------------
Name: Pedro Silva
Email: pmiguelfs@gmail.com
Date Log:
14/04/2017 - Created a few basic functions to create maps, rooms and characters
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
RoomPtr CreateRoom(string roomName, EventPtr roomEvent, OmenPtr roomOmen)
{
	RoomPtr aux = (RoomPtr)malloc(sizeof(Room));

	aux->name = roomName;
	aux->event = roomEvent;
	aux->omen = roomOmen;
	//aux->characterList = NULL;
	//aux->next = NULL;

	return aux;
}

//Allocates memory for a Character and gives value to it's atributes.
CharacterPtr CreateChar(string charName, int charMight, int charSpeed, int charSanity, int charInteligence)
{
	CharacterPtr aux = (CharacterPtr)malloc(sizeof(Character));

	aux->name = charName;
	aux->might = charMight;
	aux->speed = charSpeed;
	aux->sanity = charSanity;
	aux->inteligence = charInteligence;
	aux->history = NULL;
	aux->minions = NULL;
	aux->items = NULL;
	aux->next = NULL;
	
	return aux;
}