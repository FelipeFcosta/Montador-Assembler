#include "includes.h"
#define END_OF_FILE "-1"

class Source {
public:
	static Source& GetInstance();
	string readLine(int i);
	void insertLine(int i, string line);
	void removeLine(int i);
	static void setFile(string file);
	static string format(string line);

	int getQtdLines();
	string getFileName();


private:
	Source();

	static int qtdLines;
	static Source* instance;
	static string fileName;
	static vector<string> code;
	static vector<string> dataCode;
};