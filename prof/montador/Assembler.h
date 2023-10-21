#include "includes.h"

class Assembler {
public:
	Assembler();
	string process();
	void firstPass();

	void fillMacro(vector<string> &macro);
	void preProcess();
	string secondPass();


private:
	int searchLabel(string label);
	void insertLabel(string label);

	static map<string, string> instructionTable;
	static vector<pair<string, string>> symbolTable;
	static const vector<string> directives;

	int positionCounter;
	int lineCounter;
};
