#include "includes.h"

class Instruction {
public:
	Instruction(string instruction);

    string label, operation, op1, op2, op3;
	string arit1, arit2;

private:
	void split(string instruction);

};