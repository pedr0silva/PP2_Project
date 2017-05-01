/*
File: main.c
Description:
14/04/2017 - This file will be the playground for a while.
------------//---------------//------------//---------------//------------//---------------
Name: Pedro Silva
Email: pmiguelfs@gmail.com
Date Log:
------------//---------------//------------//---------------//------------//---------------
Name: Diogo Portela
Email:
Date Log:
*/

#include <stdio.h>
#include "estructs.h"

int main(void)
{

	CardPtr database = CreateDatabase();
	AddCards(*database);

	return 0;
}