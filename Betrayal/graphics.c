#include "estructs.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

char* InitString(char* string)
{
	for (int i = 0; i < MAXSTRING; i++)
	{
		string[i] = 0;
	}
	return string;
}
char* ToUpper(char* string)
{
	for (int i = 0; string[i] != '\0'; i++)
	{
		string[i] = toupper(string[i]);
	}
	return string;
}
char* ReadInput()
{
	char input[MAXSTRING];
	InitString(input);
	printf("Input: ");
	gets(input);
	ToUpper(input);

	return input;
}
void InputBreak()
{
	printf("Press a key to continue.");
	getchar();
	system("cls");
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