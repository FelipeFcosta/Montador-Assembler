#include "includes.h"
#include "Source.h"
#include "Assembler.h"
#include "Instruction.h"
#include "Error.h"


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

	string outputFile = inputFile.substr(0, inputFile.size()-4) + ".obj";
	ofstream objFile(outputFile);
	objFile << obj;
	objFile.close();

	if (Error::count > 0) {
		cout << Error::count << " erros: falha ao compilar " << inputFile << endl;
	} else {
		cout << Error::count << "Arquivo compilado sem falhas -> " << outputFile << endl;
	}

	return 0;
}