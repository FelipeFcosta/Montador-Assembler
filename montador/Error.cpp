#include "Error.h"

int Error::count = 0;

void Error::print(TYPE errorType, int line, string errorMsg, string instruction) {
	string error = "";
	switch (errorType)
	{
	case LEXICAL:
		error = "léxico";
	break;
	case SYNTACTIC:
		error = "sintático";
	break;
	default:
		error = "semântico";
		break;
	}

	cout << "\033[1;31m Erro " << error << ":\033[0m " << errorMsg << endl;
	printf ("  %02d", line);
	cout << " | \t" << instruction << endl;
	count++;
}