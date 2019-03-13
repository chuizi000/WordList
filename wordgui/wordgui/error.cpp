#include <cstdio>
#include <cstdlib>
#include "error.h"

void error(int e_code)
{

	switch (e_code)
	{
	case 0:
		throw "Incorrect command line parameters!";
		break;
	case 1:
		throw "Command line arguments include both -w and -c";
		break;
	case 2:
		throw "Command line parameters do not contain - w or -c";
		break;
	case 3:
		throw "The parameter should be a letter";
		break;
	case 4:
		throw "There is a ring in the word list";
		break;
	case 5:
		throw "Missing parameters";
		break;
	case 6:
		throw "File not found";
		break;
	default:
		throw "unknown error";
		break;
	}
	return;
}
