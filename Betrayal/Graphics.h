#pragma once
#include "Include\Structs.h"

#define MAX_HEIGHT 29
#define MAX_WIDTH 120

typedef enum {
	WALL_HORIZONTAL = '-',
	WALL_VERTICAL = '|',
	WALL_TOP_CORNER = '.',
	WALL_BOTTOM_CORNER = '\'',
	WALL_SIDE_CORNER = ':',
	WALL_CENTER = '+',
} WALLS;

typedef enum
{
	NONE,
	UP_ARROW = 72,
	DOWN_ARROW = 80,
	LEFT_ARROW = 75,
	RIGHT_ARROW = 77,
	ESC = 27,
	ENTER = 13,
	SPACE = 32,
}KEYBOARD;

struct camera
{
	Vector2 MinBound;
	int minLenght;
	char viewPort[MAX_HEIGHT][MAX_WIDTH];
};
typedef struct camera Camera, *CameraPtr;