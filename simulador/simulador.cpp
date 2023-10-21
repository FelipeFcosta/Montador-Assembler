#include "includes.h"

vector<int> memory;
int PC = 0;
int ACC;

enum OPCODES {
	_, ADD, SUB, MUL, DIV, JMP, JMPN, JMPP, JMPZ, COPY,
	LOAD, STORE, INPUT, OUTPUT, STOP
};

int main(int argc, char *argv[]) {

	string fileName;

	if (argc >= 2) {
		if (argc >= 2) {
			fileName = argv[1];
		} else {
			cout << "Arquivo de entrada nao encontrado, execute com o formato \"./simulator /path/to/myprogram.obj\"" << endl;
			exit(0);
		}
	}

	double v;
	ifstream file(fileName);

	if (file) {
		int value;
		while (file >> value)
			memory.push_back(value);
	} else {
		cout << "Erro ao abrir arquivo " << fileName << endl;
		exit(0);
	}

	bool firstPass = true;
	bool stop = false;
	bool segmentationFault = false;
	int dataStart = 0;
	ACC = 0;
	while (firstPass || !stop) {
		int instruction = memory[PC];
		int prevPC = PC;
		switch (instruction) {
		case ADD:
			ACC += memory[memory[PC+1]];
			PC += 2;
			break;
		case SUB:
			ACC -= memory[memory[PC+1]];
			PC += 2;
			break;
		case MUL:
			ACC *= memory[memory[PC+1]];
			PC += 2;
			break;
		case DIV:
			ACC /= memory[memory[PC+1]];
			PC += 2;
			break;
		case JMP:
			if (!firstPass) PC = memory[PC+1];
			else	PC += 2;
			if (!firstPass && PC >= dataStart) segmentationFault = true;
			break;
		case JMPN:
			if (!firstPass && ACC < 0) {
				PC = memory[PC+1];
				if (!firstPass && PC >= dataStart) segmentationFault = true;
			} else {
				PC += 2;
			}
			break;
		case JMPP:
			if (!firstPass && ACC > 0) {
				PC = memory[PC+1];
				if (!firstPass && PC >= dataStart) segmentationFault = true;
			} else {
				PC += 2;
			}
			break;
		case JMPZ:
			if (!firstPass && ACC == 0) {
				PC = memory[PC+1];
				if (!firstPass && PC >= dataStart) segmentationFault = true;
			} else {
				PC += 2;
			}
			break;
		case COPY:
			if (!firstPass) memory[memory[PC+2]] = memory[memory[PC+1]];
			PC += 3;
			break;
		case LOAD:
			ACC = memory[memory[PC+1]];
			PC += 2;
			break;
		case STORE:
			if (!firstPass) memory[memory[PC+1]] = ACC;
			PC += 2;
			break;
		case INPUT:
			if (!firstPass) {
				cout << "- ";
				cin >> memory[memory[PC+1]];
				cin.get();
			}
			PC += 2;
			break;
		case OUTPUT:
			if (!firstPass) {
				cout << "PC <- " << prevPC << " ACC <- " << ACC << " SAIDA: " << memory[memory[PC+1]];
			} 
			PC += 2;
			break;
		case STOP:
			stop = true;
			PC += 1;
			break;
		default:
			if (!firstPass) {
				cout << "Instrução \'" <<  instruction << "\' não reconhecida" << endl;
				stop = true;
			}
			break;
		}

		if (!firstPass && memory[prevPC] != OUTPUT) {
			cout << "PC <- " << prevPC << " ACC <- " << ACC;
			if (segmentationFault) stop = true;
		}

		if (segmentationFault) {
			cin.get();
			cout << "\033[1;31mSegmentation Fault\033[0m (PC <- " << PC << ")" << endl;
		}


		if (!firstPass && !segmentationFault) {
			cin.get();
		}

		if (firstPass && stop == true) {
			dataStart = PC;
			firstPass = false;
			stop = false;
			PC = 0;
			ACC = 0;
		}
	}


	return 0;
}