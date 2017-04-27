/*
File: functions.c
Description: This file has the needed data structures to develop and run the game.
------------//---------------//------------//---------------//------------//---------------
Name: Pedro Silva
Email: pmiguelfs@gmail.com
Date Log:
12/04/2017 - Created a few auxiliar definitions, like boolean and string.
		   - Began working on the game structures (Created Minion, Omen, Item and Character).

14/04/2017 - Created the rest of the necessary structs (they might need some work tho).
------------//---------------//------------//---------------//------------//---------------
Name: Diogo Portela
Email:
Date Log:

*/

#include <stdio.h>
#define MAX_CARDS 
#define MAX_OMENS 13
#define MAX_ITEMS 22
#define MAX_EVENTS 45
#define MAX_CHARACTERS 12
#define MAX_ROOMS 44

//------------------------------------GENERALS-------------------------------------//
#ifndef SIGNATURES
#define SIGNATURES

struct Minion;
struct Omen;
struct Item;
struct Event;
struct Character;
struct Card;

#endif // !SIGNATURES

#ifndef STRING
#define STRING

typedef char *string;

#endif // ! STRING

#ifndef BOOLEAN
#define BOOLEAN

typedef enum {FALSE, TRUE} boolean;

#endif // !BOOLEAN

#ifndef HISTORY
#define HISTORY

struct History
{
	string turn;
	struct History *next;
};

typedef struct History History, *HistoryPtr;

#endif // !HISTORY

//-----------------------------------END-GENERALS-------------------------------------//

//------------------------------------MAP-------------------------------------//
#ifndef FLOOR
#define FLOOR

typedef enum { BASEMENT = -1, GROUND, UPPER } Floor;

#endif // !FLOOR

#ifndef DIRECTION
#define DIRECTION

typedef enum direction { Up, Right, Down, Left } Direction;

#endif // !Direction

#ifndef WALLTYPE
#define WALLTYPE

typedef enum wallType { Empy, Door, Window } WallType;

#endif // !Direction

#ifndef ROOMWALL
#define ROOMWALL

struct roomWall
{
	Direction Direction;
	WallType WallType;
};

typedef struct roomWall RoomWall;
#endif // !RoomWall

#ifndef ROOM
#define ROOM

struct Room
{
	string name;
	RoomWall Wall[4];
	EventPtr event;
	OmenPtr omen;
	struct Room *Up, *Down, *Left, *Right, *Above, *Below;
};

typedef struct Room Room, *RoomPtr;

#endif // !ROOM

#ifndef MAP
#define MAP

struct Map
{
	Floor mapFloor;
	RoomPtr roomList;
	int roomCounter;
	struct Map *next;
	struct Map *prev;
};

typedef struct Map Map, *MapPtr;

#endif // !MAP
//-----------------------------------END-MAP----------------------------------//

//------------------------------------GAME-ITEMS-------------------------------------//
#ifndef MINION
#define MINION

struct Minion
{
	string name;
	int might_mod, speed_mod, sanity_mod, intellect_mod;
	//usamos a tag mod porque os minions nao teem stats eles proprios, mas modificam as stats do seu "mestre".
	struct Minion *next;
};

typedef struct Minion Minion, *MinionPtr;

#endif // !MINION

#ifndef OMEN
#define OMEN

struct Omen
{
	string name, description;
	int might_mod, speed_mod, sanity_mod, intellect_mod;
	struct Omen *next;
};

typedef struct Omen Omen, *OmenPtr;

#endif // !OMEN

#ifndef ITEM
#define ITEM

struct Item
{
	string name, description;
	int might_mod, speed_mod, sanity_mod, intellect_mod;
	struct Item *next;
};

typedef struct Item Item, *ItemPtr;

#endif // !ITEM

#ifndef EVENT
#define EVENT

struct Event
{
	string name, description;
	int might_mod, speed_mod, sanity_mod, intellect_mod;
	struct Event *next;
};

typedef struct Event Event, *EventPtr;

#endif // !EVENT

#ifndef CHARACTER
#define CHARACTER

struct Character
{
	string name;
	int might, speed, sanity, inteligence;
	MinionPtr minions;
	ItemPtr items;
	HistoryPtr history;
	RoomPtr room;
	struct Character *next;
};

typedef struct Character Character, *CharacterPtr;

#endif // !CHARACTER

#ifndef CARDS
#define CARDS

struct Card
{
	Item itemList[MAX_ITEMS];
	Omen omenList[MAX_OMENS];
	Event eventList[MAX_EVENTS];
	Character characterList[MAX_CHARACTERS];
};

typedef struct Card Card, *CardPtr;

#endif // !CARDS
//-----------------------------END-GAME-ITEMS----------------------------------//

#ifndef MASTER
#define MASTER

struct Master
{
	CharacterPtr characterList;
	MapPtr head;
};

#endif // !MASTER