#include "includes.h"
#include "Source.h"
#include "Assembler.h"
#include "Instruction.h"


int main(int argc, char *argv[]) {
	string inputFile;
	if (argc >= 2) {
		inputFile = argv[1];
	} else {
		cout << "Arquivo de entrada nao encontrado, execute com o formato \"./montador /path/to/myprogram.asm\"" << endl;
		exit(0);
	}

	Source::setFile(inputFile);

	Assembler assembler;
	string obj = assembler.process();

	cout << obj << endl;

	ofstream objFile(inputFile.substr(0, inputFile.size()-4) + ".obj");
	objFile << obj;
	objFile.close();

	return 0;
}