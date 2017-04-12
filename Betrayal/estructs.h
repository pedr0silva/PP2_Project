#include <stdio.h>
#define MAX_CARDS 31

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

#ifndef ITEM
#define ITEM

struct Item
{
	string name, description;
	int might_mod, speed_mod, sanity_mod, intellect_mod;
	struct Item *next;
};

#endif // !ITEM


#ifndef CHARACTER
#define CHARACTER

struct Character
{
	string name;
	int might, speed, sanity, intellect;
	struct Minion *minions;
	struct Items *items;
};

#endif // !CHARACTERS


#ifndef CARDS
#define CARDS

struct Card
{
	
	string events[MAX_CARDS];
	string omens[MAX_CARDS];
};

#endif // !CARDS


