#include "estructs.h"
#include <conio.h>


stringPtr ReadInput()
{
	string input;
	InitString(&input);
	printf("Input: ");
	gets(input);
	strupr(input);
	return (stringPtr)input;
}
void InputBreak()
{
	printf("Press a key to continue.");
	getch();
	system("cls");
}
boolean InsertLineInDrawingTable(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH], Vector2Ptr position, stringPtr text)
{

	if (position->x > MAX_WIDTH || position->y > MAX_HEIGHT || position->x < 0 || position->y < 0)
		return FALSE;
	strcpy((*drawingTable)[position->y][position->x], text);
	return TRUE;
}

void CleanDrawingTable(char(*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	for (int i = 0; i < MAX_HEIGHT; i++)
		for (int j = 0; j < MAX_WIDTH; j++)
			(*drawingTable)[i][j] = ' ';
}
int UpdateMap(Master *mainStruct, char *drawingTable[MAX_HEIGHT][MAX_WIDTH])
{
	
}
int DrawMap(char (*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	for (int i = 0; i < MAX_HEIGHT; i++)
		for (int j = 0; j < MAX_WIDTH; j++)
			printf("%c", (*drawingTable)[i][j]);
}
void Menu(MasterPtr master, char (*drawingTable)[MAX_HEIGHT][MAX_WIDTH])
{
	char* input = NULL;

 	printf("WELCOME TO BETRAYAL AT HOUSE ON THE HILL\n\n");
	InputBreak();

	do
	{
		printf(	"Start\n"
				"Exit\n\n");
		input = ReadInput();

	} while ((strcmp(input, "EXIT") != 0));
	InputBreak();

	return 1;
}