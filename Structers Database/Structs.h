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

#pragma region GENERAL
	#pragma region CONSTANTS
		#ifndef MAX_CARDS
		#define MAX_CARDS 6
		#endif // !MAX_CARDS
		#ifndef MAX_OMENS
		#define MAX_OMENS 13
		#endif // !MAX_OMENS
		#ifndef MAX_ITEMS
		#define MAX_ITEMS 22
		#endif // !MAX_ITEMS
		#ifndef MAX_EVENTS
		#define MAX_EVENTS 45
		#endif // !MAX_EVENTNS
		#ifndef MAX_MINIONS
		#define MAX_MINIONS 3
		#endif // !MAX_MINIONS
		#ifndef MAX_CHARACTERS
		#define MAX_CHARACTERS 12
		#endif // !MAX_CHARACTERS
		#ifndef MAX_ROOMS
		#define MAX_ROOMS 50
		#endif // !MAX_ROOMS
		#ifndef WALL_NUMBER
		#define WALL_NUMBER 4
		#endif // !WALL_NUMBER
		#ifndef MAX_STRING
		#define MAX_STRING 512
		#endif // !MAX_STRING
		#ifndef ROOM_SIZE
		#define ROOM_SIZE 5
		#endif // ROOM_SIZE
		#ifndef MAX_SAVES
		#define MAX_SAVES 3
		#endif // !MAX_SAVES


	#pragma endregion
	#pragma region AUXILIARY STRUCTS
		#ifndef STRING
			#define STRING
			typedef char string[MAX_STRING];
		#endif // !STRING
		#ifndef TRUE
			#define TRUE 1
			#define FALSE 0
			typedef int BOOL;
		#endif // TRUE
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
		BOOL isUsed;
		struct room *next;
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
		string name;
		int might, speed, sanity, inteligence;
		int playerNumber;
		Vector2 position;
		RoomPtr room;
		MinionPtr minions;
		ItemPtr items;
		HistoryPtr history;
		struct Character *next;
	};
	struct Card
	{
		Character characterList[MAX_CHARACTERS];
		unsigned int charCount;
		Event eventList[MAX_EVENTS];
		unsigned int eventCount;
		Item itemList[MAX_ITEMS];
		unsigned int itemCount;		
		Minion minionList[MAX_MINIONS];
		unsigned int minionCount;
		Omen omenList[MAX_OMENS];
		unsigned int omenCount;
		Room roomList[MAX_ROOMS];
		unsigned int roomCount;
	};
#pragma endregion

#pragma region MASTER
	struct Master
	{
		Card cards;
		Map map;
		CharacterPtr characterList;
		int omenTrack;
	};
	typedef struct Master Master, *MasterPtr;
#pragma endregion

#pragma region FUNCTION_SIGNATURES
	#pragma region CONSTRUCTORS
		__declspec(dllexport) Vector2Ptr CreateVector2(int x, int y);
		__declspec(dllexport) Vector2 ChangeVector2(int x, int y);
		__declspec(dllexport) Vector2Ptr DestroyVector2(Vector2Ptr node);
		__declspec(dllexport) void InitString(string string);
		__declspec(dllexport) HistoryPtr CreateHistory(string text);
		__declspec(dllexport) HistoryPtr AddHistoryToList(HistoryPtr head, HistoryPtr node);
		__declspec(dllexport) HistoryPtr RemoveHistoryFromList(HistoryPtr head, HistoryPtr node);
		__declspec(dllexport) HistoryPtr DestroyHistory(HistoryPtr room);
		__declspec(dllexport) HistoryPtr DestroyHistoryList(HistoryPtr head);

		__declspec(dllexport) RoomPtr RotateWalls(RoomPtr room, int value);
		__declspec(dllexport) RoomPtr CreateRoom(string roomName, EventPtr roomEvent, OmenPtr roomOmen, WallType upType, WallType leftType, WallType downType, WallType rightType);
		__declspec(dllexport) RoomPtr InstanciateRoom(RoomPtr room, Vector2 position);
		__declspec(dllexport) RoomPtr AddRoomToList(RoomPtr head, RoomPtr node);
		__declspec(dllexport) BOOL AddRoomToArray(MasterPtr master, RoomPtr node);
		__declspec(dllexport) BOOL RemoveRoomFromArray(MasterPtr master, string node);
		__declspec(dllexport) RoomPtr RemoveRoomFromList(RoomPtr head, string name);
		__declspec(dllexport) RoomPtr DestroyRoom(RoomPtr room);
		__declspec(dllexport) RoomPtr DestroyRoomList(RoomPtr head);
		__declspec(dllexport) RoomPtr RandomRoom(MasterPtr master);
		__declspec(dllexport) BOOL OpenRoom(MasterPtr master, FloorPtr floor, RoomPtr currentRoom, Direction direction);

		__declspec(dllexport) FloorPtr CreateFloor(FloorLevel level);
		__declspec(dllexport) FloorPtr AddFloorToList(FloorPtr head, FloorPtr node);
		__declspec(dllexport) FloorPtr RemoveFloorFromList(FloorPtr head, FloorLevel level);
		__declspec(dllexport) FloorPtr DestroyFloor(FloorPtr floor);
		__declspec(dllexport) FloorPtr DestroyFloorList(FloorPtr head);

		__declspec(dllexport) MinionPtr CreateMinion(string name, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod);
		__declspec(dllexport) BOOL AddMinionToArray(MasterPtr master, MinionPtr node);
		__declspec(dllexport) BOOL RemoveMinionFromArray(MasterPtr master, string node);
		__declspec(dllexport) MinionPtr DestroyMinion(MinionPtr node);
		__declspec(dllexport) CharacterPtr AssignMinion(CharacterPtr player, MinionPtr minion);
		__declspec(dllexport) BOOL UnassignMinion(CharacterPtr player, MinionPtr minion);

		__declspec(dllexport) EventPtr Create_Event(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod);
		__declspec(dllexport) EventPtr AddEventToList(EventPtr head, EventPtr node);
		__declspec(dllexport) BOOL AddEventToArray(MasterPtr master, EventPtr node);
		__declspec(dllexport) BOOL RemoveEventFromArray(MasterPtr master, string node);
		__declspec(dllexport) EventPtr RemoveEventFromList(EventPtr head, string name);
		__declspec(dllexport) EventPtr DestroyEvent(EventPtr node);
		__declspec(dllexport) EventPtr DestroyEventList(EventPtr head);
		__declspec(dllexport) EventPtr RandomEvent(MasterPtr master);

		__declspec(dllexport) OmenPtr CreateOmen(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod);
		__declspec(dllexport) OmenPtr AddOmenToList(OmenPtr head, OmenPtr node);
		__declspec(dllexport) BOOL AddOmenToArray(MasterPtr master, OmenPtr node);
		__declspec(dllexport) BOOL RemoveOmentFromArray(MasterPtr master, string node);
		__declspec(dllexport) OmenPtr RemoveOmenFromList(OmenPtr head, string name);
		__declspec(dllexport) OmenPtr DestroyOmen(OmenPtr node);
		__declspec(dllexport) OmenPtr DestroyOmenList(OmenPtr head);
		__declspec(dllexport) OmenPtr RandomOmen(MasterPtr master);

		__declspec(dllexport) ItemPtr CreateItem(string name, string description, int might_mod, int speed_mod, int sanity_mod, int inteligence_mod);
		__declspec(dllexport) ItemPtr AddItemToList(ItemPtr head, ItemPtr node);
		__declspec(dllexport) BOOL AddItemToArray(MasterPtr master, ItemPtr node);
		__declspec(dllexport) BOOL RemoveItemFromArray(MasterPtr master, string node);
		__declspec(dllexport) ItemPtr RemoveItemFromList(ItemPtr head, string name);
		__declspec(dllexport) ItemPtr DestroyItem(ItemPtr node);
		__declspec(dllexport) ItemPtr DestroyItemList(ItemPtr head);
		__declspec(dllexport) ItemPtr RandomItem(MasterPtr master);
		__declspec(dllexport) CharacterPtr AssignItem(CharacterPtr player, ItemPtr item);
		__declspec(dllexport) BOOL UnassignItem(CharacterPtr player, ItemPtr item);

		__declspec(dllexport) CharacterPtr CreateChar(string name, int might, int speed, int sanity, int inteligence);
		__declspec(dllexport) CharacterPtr AddCharToList(CharacterPtr head, CharacterPtr node);
		__declspec(dllexport) BOOL AddCharToArray(MasterPtr master, CharacterPtr node);
		__declspec(dllexport) CharacterPtr InstanciateChar(CharacterPtr character, RoomPtr startingRoom, Vector2 position, int playerNumber);
		__declspec(dllexport) BOOL RemoveCharFromArray(MasterPtr master, string node);
		__declspec(dllexport) CharacterPtr RemoveCharFromList(CharacterPtr head, string name);
		__declspec(dllexport) CharacterPtr DestroyChar(CharacterPtr node);
		__declspec(dllexport) CharacterPtr DestroyCharList(CharacterPtr head);
		__declspec(dllexport) CharacterPtr RandomChar(MasterPtr master);
	#pragma endregion
	#pragma region ROLLS / STATS
		__declspec(dllexport) unsigned int DiceRoll(int stat);
	#pragma endregion
	#pragma region DATABASE
		__declspec(dllexport) BOOL Reset(MasterPtr master);
		__declspec(dllexport) BOOL LoadMaster(MasterPtr master, string fileName);
		__declspec(dllexport) BOOL EndMaster(MasterPtr master, string fileName);
		__declspec(dllexport) string* ReadSaveDirectory();
	#pragma endregion
#pragma endregion

#endif // !BETRAYAL_STRUCTS