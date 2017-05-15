#include "estructs.h"
#include <string.h>
#include <ctype.h>
#include <conio.h>


string InitString(string string)
{
	for (int i = 0; i < MAXSTRING; i++)
	{
		string[i] = 0;
	}
	return string;
}
string ToUpper(string string)
{
	for (int i = 0; string[i] != '\0'; i++)
	{
		string[i] = toupper(string[i]);
	}
	return string;
}
string ReadInput()
{
	char input[MAXSTRING];
	InitString(input);
	printf("Input: ");
	gets(input);
	ToUpper(input);

	return (string)input;
}
void InputBreak()
{
	printf("Press a key to continue.");
	getchar();
	system("cls");
}

int UpdateMap(Master *mainStruct, char *mapText[MAX_HEIGHT][MAX_WIDTH])
{

}
int DrawMap(char *map[MAX_HEIGHT][MAX_WIDTH])
{
	char aux;
	for (int i = 0; i < MAX_HEIGHT; i++)
		for (int j = 0; i < MAX_WIDTH; j++)
			printf("%c", map[i][j]);
}
int Menu()
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