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

	string outputFile = inputFile.substr(0, inputFile.size()-4) + ".obj";
	ofstream objFile(outputFile);
	objFile << obj;
	objFile.close();

	if (Error::count > 0) {
		cout << Error::count << " erros: Arquivo objeto não gerado" << endl;
	} else {
		cout << "Arquivo compilado com sucesso -> '" << outputFile << '\'' << endl;
	}

	return 0;
}