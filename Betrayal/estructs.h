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

27/04/2017 - Minor Fixes.

28/04/2017 - Minor Fixes.

01/04/2017 - Added some functions' signatures.
		   - Minor Fixes.
------------//---------------//------------//---------------//------------//---------------
Name: Diogo Portela
Email:
Date Log:
27/04/2017 - Minor Fixes
*/

#include <stdio.h>
#define MAX_CARDS 136
#define MAX_OMENS 13
#define MAX_ITEMS 22
#define MAX_EVENTS 45
#define MAX_CHARACTERS 12
#define MAX_ROOMS_X 10
#define MAX_ROOMS_Y 10
#define MAX_ROOMS_Z 3
#define MAX_WIDTH 120
#define MAX_HEIGHT 30
#define WALL_NUMBER 4

//------------------------------------GENERALS-------------------------------------//
#ifndef STRUCTSSIGNATURES
#define STRUCTSSIGNATURES

typedef struct Minion Minion, *MinionPtr;
typedef struct Omen Omen, *OmenPtr;
typedef struct Item Item, *ItemPtr;
typedef struct Event Event, *EventPtr;
typedef struct Character Character, *CharacterPtr;
typedef struct Card Card, *CardPtr;

#endif // !SIGNATURES

#ifndef STRING
#define STRING

typedef char *string;

#endif // ! STRING

#ifndef BOOLEAN
#define BOOLEAN

typedef enum { FALSE, TRUE } boolean;

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

#ifndef POSITION
#define POSITION

struct vector3
{
	int x;
	int y;
	int z;
};

typedef struct vector3 Vector3, Vector3Ptr;
#endif // !POSITION

//-----------------------------------END-GENERALS-------------------------------------//

//------------------------------------MAP-------------------------------------//

#ifndef DIRECTION
#define DIRECTION

typedef enum direction { Up, Right, Down, Left, Above, Bellow } Direction;

#endif // !DIRECTION

#ifndef WALLTYPE
#define WALLTYPE

typedef enum wallType { Empty, Door, Window } WallType;

#endif // !WALLTYPE

#ifndef ROOMWALL
#define ROOMWALL

struct roomWall
{
	Direction Direction;
	WallType WallType;
};

typedef struct roomWall RoomWall, *RoomWallPtr;

#endif // !ROOMWALL

#ifndef ROOM
#define ROOM

struct Room
{
	Vector3 position;
	int positionLenght;
	string name;
	RoomWall wall[WALL_NUMBER];
	EventPtr event;
	OmenPtr omen;
	struct Room *next;
};

typedef struct Room Room, *RoomPtr;

#endif // !ROOM

#ifndef FLOOR
#define FLOOR

typedef enum { BASEMENT = -1, GROUND, UPPER } FloorLevel;

struct floor
{
	FloorLevel level;
	RoomPtr roomList;
	struct floor *next;
};
typedef struct floor Floor, *FloorPtr;

#endif // !FLOOR

#ifndef MAP
#define MAP

struct Map
{
	int roomCounter;
	FloorPtr mapFloor;
	//RoomPtr roomList;
	//Room roomList[MAX_ROOMS_X][MAX_ROOMS_Y][MAX_ROOMS_Z];
	//struct Map *next;
	//struct Map *prev;
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

#endif // !MINION

#ifndef OMEN
#define OMEN

struct Omen
{
	string name, description;
	int might_mod, speed_mod, sanity_mod, intellect_mod;
	struct Omen *next;
};

#endif // !OMEN

#ifndef ITEM
#define ITEM

struct Item
{
	string name, description;
	int might_mod, speed_mod, sanity_mod, intellect_mod;
	struct Item *next;
};

#endif // !ITEM

#ifndef EVENT
#define EVENT

struct Event
{
	string name, description;
	int might_mod, speed_mod, sanity_mod, intellect_mod;
	struct Event *next;
};

#endif // !EVENT

#ifndef CHARACTER
#define CHARACTER

struct Character
{
	Vector3 position;
	string name;
	int might, speed, sanity, inteligence;
	MinionPtr minions;
	ItemPtr items;
	HistoryPtr history;
	RoomPtr room;
	struct Character *next;
};

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

#endif // !CARDS
//-----------------------------END-GAME-ITEMS----------------------------------//

#ifndef MASTER
#define MASTER

struct Master
{
	int damageTrack;
	CharacterPtr characterList;
	MapPtr head;
};

typedef struct Master Master;

#endif // !MASTER

#ifndef SIGNATURES
#define SIGNATURES

#pragma region CONSTRUCTORS

CardPtr CreateDatabase(void);
MapPtr CreateMap(Floor mapFloor);
CharacterPtr CreateChar(string name, int might, int speed, int sanity, int inteligence);
EventPtr CreateEvent(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod);
OmenPtr CreateOmen(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod);
ItemPtr CreateItem(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod);

#pragma endregion

#pragma region ROLLS / STATS

unsigned int DiceRoll(int stat);

#pragma endregion

#pragma region ITEM / MINION ASSIGNMENT

CharacterPtr AsignMinion(CharacterPtr player, MinionPtr minion);
boolean UnasignMinion(CharacterPtr player, MinionPtr minion);
CharacterPtr AsignItem(CharacterPtr player, ItemPtr item);
boolean UnasignItem(CharacterPtr player, ItemPtr item);

#pragma endregion

#pragma region DATABASE

void AddCards(Card c);
void LoadCards(Card c);

#pragma endregion

#pragma region UI
#define MAXSTRING 250

string InitString(string string);
string ToUpper(string string);
string ReadInput();
void InputBreak();
int Menu();

#pragma endregion


#endif // !SIGNATURES
