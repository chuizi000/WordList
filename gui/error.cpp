#include <cstdio>
#include <cstdlib>
#include "error.h"

void error(int e_code)
{

	switch (e_code)
	{
	case 0:
		printf("Incorrect command line parameters!");
		break;

	case 1:
		printf("Command line arguments include both -w and -c!");
		break;

	case 2:
		printf("Command line parameters do not contain - w or -c!");
		break;

	case 3:
		printf("The parameter should be a letter!");
		break;

	case 4:
		printf("There is a ring in the word list!");
		break;

	case 5:
		printf("Missing parameters!");
		break;

	case 6:
		printf("File not found!");
		break;

	default:

		break;
	}

	exit(1);
	return;
}
