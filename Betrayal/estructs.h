#include <stdio.h>
#define MAX_OMENS 13
#define MAX_ITEMS 22
#define MAX_EVENTS 45
#define MAX_CHARACTERS 12

#ifndef  STRING
#define STRING

typedef char *string;

#endif // ! STRING

#ifndef BOOLEAN
#define BOOLEAN

typedef enum {FALSE, TRUE} boolean;

#endif // !BOOLEAN

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
	//int might_mod, speed_mod, sanity_mod, intellect_mod;
	struct Event *next;
};

typedef struct Event Event, *EventPtr;

#endif // !EVENT

#ifndef HISTORY
#define HISTORY

struct History
{
	string turn;
	struct History *next;
};

typedef struct History History, *HistoryPtr;

#endif // !HISTORY


#ifndef CHARACTER
#define CHARACTER

struct Character
{
	string name;
	int might, speed, sanity, intellect;
	Minion *minions;
	Item *items;
	History *history;
	struct Character *next;
};

typedef struct Character Character, *CharacterPtr;

#endif // !CHARACTERS


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
