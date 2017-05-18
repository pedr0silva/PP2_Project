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

#ifndef BETRAYAL_STRUCTS
#define BETRAYAL_STRUCTS

#include <stdio.h>
#include <string.h>
#include <Windows.h>

#pragma region GENERAL

#pragma region CONSTANTS
#define MAX_CARDS 6
#define MAX_OMENS 13
#define MAX_ITEMS 22
#define MAX_EVENTS 45
#define MAX_MINIONS 3
#define MAX_CHARACTERS 12
#define MAX_ROOMS 45
#define MAX_WIDTH 120
#define MAX_HEIGHT 29
#define WALL_NUMBER 4
#define MAX_STRING 512
#pragma endregion

#pragma region AUXILIARY STRUCTS
typedef char string[MAX_STRING];

typedef enum { _FALSE, _TRUE } _boolean;

struct history
{
	string turn;
	struct History *next;
};
typedef struct history History, *HistoryPtr;

struct vector2
{
	int x;
	int y;
};
typedef struct vector2 Vector2, *Vector2Ptr;
#pragma endregion

typedef struct Minion Minion, *MinionPtr;
typedef struct Omen Omen, *OmenPtr;
typedef struct Item Item, *ItemPtr;
typedef struct Event Event, *EventPtr;
typedef struct Character Character, *CharacterPtr;
typedef struct Card Card, *CardPtr;

#pragma endregion

#pragma region MAP

typedef enum direction { UP, RIGHT, DOWN, LEFT, ABOVE, BELLOW } Direction;

typedef enum wallType { EMPTY, DOOR, WINDOW } WallType;

struct roomWall
{
	Direction Direction;
	WallType WallType;
};
typedef struct roomWall RoomWall, *RoomWallPtr;

struct room
{
	Vector2 position;
	int positionLenght;
	string name;
	RoomWall wall[WALL_NUMBER];
	EventPtr event;
	OmenPtr omen;
	struct Room *next;
};
typedef struct room Room, *RoomPtr;

typedef enum { BASEMENT = -1, GROUND, UPPER } FloorLevel;

struct floor
{
	FloorLevel level;
	RoomPtr roomList;
	struct floor *next;
};
typedef struct floor Floor, *FloorPtr;

struct Map
{
	int roomCounter;
	FloorPtr mapFloor;
};
typedef struct Map Map, *MapPtr;

#pragma endregion

#pragma region  GAME_ITEMS

struct Minion
{
	string name;
	int might_mod, speed_mod, sanity_mod, intellect_mod;
	//usamos a tag mod porque os minions nao teem stats eles proprios, mas modificam as stats do seu "mestre".
	struct Minion *next;
};
struct Omen
{
	string name, description;
	int might_mod, speed_mod, sanity_mod, intellect_mod;
	struct Omen *next;
};
struct Item
{
	string name, description;
	int might_mod, speed_mod, sanity_mod, intellect_mod;
	struct Item *next;
};
struct Event
{
	string name, description;
	int might_mod, speed_mod, sanity_mod, intellect_mod;
	struct Event *next;
};
struct Character
{
	Vector2 position;
	RoomPtr room;
	string name;
	int might, speed, sanity, inteligence;
	MinionPtr minions;
	ItemPtr items;
	HistoryPtr history;
	struct Character *next;
};
struct Card
{
	Item itemList[MAX_ITEMS];
	unsigned int itemCount;
	Omen omenList[MAX_OMENS];
	unsigned int omenCount;
	Event eventList[MAX_EVENTS];
	unsigned int eventCount;
	Character characterList[MAX_CHARACTERS];
	unsigned int charCount;
	Minion minionList[MAX_MINIONS];
	unsigned int minionCount;
	Room roomList[MAX_ROOMS];
	unsigned int roomCount;
};

#pragma endregion

#pragma region MASTER

struct Master
{
	int omenTrack;
	CharacterPtr characterList;
	Map map;
	Card cards;
};
typedef struct Master Master, *MasterPtr;

#pragma endregion

#pragma region FUNCTION_SIGNATURES
	#pragma region CONSTRUCTORS

Vector2Ptr CreateVector2(int x, int y);
Vector2Ptr ChangeVector2(Vector2Ptr auxVec, int x, int y);
Vector2Ptr DestroyVector2(Vector2Ptr node);
void InitString(string string);
HistoryPtr CreateHistory(string text);
HistoryPtr AddHistoryToList(HistoryPtr head, HistoryPtr node);
HistoryPtr RemoveHistoryFromList(HistoryPtr head, HistoryPtr node);
HistoryPtr DestroyHistory(HistoryPtr room);
HistoryPtr DestroyHistoryList(HistoryPtr head);

RoomPtr RotateWalls(RoomPtr room, int value);
RoomPtr CreateRoom(string roomName, EventPtr roomEvent, OmenPtr roomOmen, WallType upType, WallType leftType, WallType downType, WallType rightType);
RoomPtr InstanciateRoom(RoomPtr room, Vector2 position);
RoomPtr AddRoomToList(RoomPtr head, RoomPtr node);
_boolean AddRoomToArray(MasterPtr master, RoomPtr node);
_boolean RemoveRoomFromArray(MasterPtr master, string node);
RoomPtr RemoveRoomFromList(RoomPtr head, string name);
RoomPtr DestroyRoom(RoomPtr room);
RoomPtr DestroyRoomList(RoomPtr head);
RoomPtr RandomRoom(MasterPtr master);
_boolean OpenRoom(MasterPtr master, FloorPtr floor, RoomPtr currentRoom, Direction direction);

FloorPtr CreateFloor(FloorLevel level);
FloorPtr AddFloorToList(FloorPtr head, FloorPtr node);
FloorPtr RemoveFloorFromList(FloorPtr head, FloorLevel level);
FloorPtr DestroyFloor(FloorPtr floor);
FloorPtr DestroyFloorList(FloorPtr head);

MinionPtr CreateMinion(string name, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod);
_boolean AddMinionToArray(MasterPtr master, MinionPtr node);
_boolean RemoveMinionFromArray(MasterPtr master, string node);
MinionPtr DestroyMinion(MinionPtr node);
CharacterPtr AssignMinion(CharacterPtr player, MinionPtr minion);
_boolean UnassignMinion(CharacterPtr player, MinionPtr minion);

EventPtr Create_Event(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod);
EventPtr AddEventToList(EventPtr head, EventPtr node);
_boolean AddEventToArray(MasterPtr master, EventPtr node);
_boolean RemoveEventFromArray(MasterPtr master, string node);
EventPtr RemoveEventFromList(EventPtr head, string name);
EventPtr DestroyEvent(EventPtr node);
EventPtr DestroyEventList(EventPtr head);
EventPtr RandomEvent(MasterPtr master);

OmenPtr CreateOmen(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod);
OmenPtr AddOmenToList(OmenPtr head, OmenPtr node);
_boolean AddOmenToArray(MasterPtr master, OmenPtr node);
_boolean RemoveOmentFromArray(MasterPtr master, string node);
OmenPtr RemoveOmenFromList(OmenPtr head, string name);
OmenPtr DestroyOmen(OmenPtr node);
OmenPtr DestroyOmenList(OmenPtr head);
OmenPtr RandomOmen(MasterPtr master);

ItemPtr CreateItem(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod);
ItemPtr AddItemToList(ItemPtr head, ItemPtr node);
_boolean AddItemToArray(MasterPtr master, ItemPtr node);
_boolean RemoveItemFromArray(MasterPtr master, string node);
ItemPtr RemoveItemFromList(ItemPtr head, string name);
ItemPtr DestroyItem(ItemPtr node);
ItemPtr DestroyItemList(ItemPtr head);
ItemPtr RandomItem(MasterPtr master);
CharacterPtr AssignItem(CharacterPtr player, ItemPtr item);
_boolean UnassignItem(CharacterPtr player, ItemPtr item);

CharacterPtr CreateChar(string name, int might, int speed, int sanity, int inteligence);
CharacterPtr AddCharToList(CharacterPtr head, CharacterPtr node);
_boolean AddCharToArray(MasterPtr master, CharacterPtr node);
_boolean RemovecharFromArray(MasterPtr master, string node);
CharacterPtr RemoveCharFromList(CharacterPtr head, string name);
CharacterPtr DestroyChar(CharacterPtr node);
CharacterPtr DestroyCharList(CharacterPtr head);
CharacterPtr RandomChar(MasterPtr master);

	#pragma endregion

	#pragma region ROLLS / STATS

unsigned int DiceRoll(int stat);

	#pragma endregion

	#pragma region DATABASE

void Reset(MasterPtr master);
void AddCards(CardPtr c);
_boolean LoadMaster(MasterPtr master);
_boolean EndMaster(MasterPtr master);

	#pragma endregion

	#pragma region UI

void ReadInput(string input);
void InputBreak();
_boolean InsertLineInDrawingTable(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH], Vector2Ptr position, string text);
_boolean CleanDrawingTable(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH]);
int DrawMap(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH]);
void Menu(MasterPtr master, char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH]);

	#pragma endregion

#pragma endregion

#endif // !BETRAYAL_STRUCTS