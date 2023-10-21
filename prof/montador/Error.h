#include "includes.h"

class Error {
public:

	enum TYPE {
		LEXICAL, SYNTACTIC, SEMANTIC
	};

	static void print(TYPE errorType, int line, string errorMsg, string instruction);

private:

};
