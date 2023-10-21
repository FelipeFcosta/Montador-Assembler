#include "includes.h"

class Error {
public:

	static int count;

	enum TYPE {
		LEXICAL, SYNTACTIC, SEMANTIC
	};

	static void print(TYPE errorType, int line, string errorMsg, string instruction);

private:
};
