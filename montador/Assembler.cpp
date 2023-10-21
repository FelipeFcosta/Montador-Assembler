#include "Assembler.h"
#include "Source.h"
#include "Instruction.h"
#include "Error.h"

#define CHECK_NUMBER(x, y) for (int i = 0; i < x.size(); i++) if (!isdigit(x[i])) y = true;
#define TO_UPPER(x) for (auto &c : x) c = toupper(c);

std::map<string, string> Assembler::instructionTable = {
    {"ADD", "1"}, {"SUB", "2"}, {"MUL", "3"}, {"DIV", "4"}, {"JMP", "5"}, {"JMPN", "6"}, {"JMPP", "7"},
    {"JMPZ", "8"}, {"COPY", "9"}, {"LOAD", "10"}, {"STORE", "11"}, {"INPUT", "12"}, {"OUTPUT", "13"}, {"STOP", "14"}
};

const std::vector<string> Assembler::directives = {"SPACE", "CONST"};
vector<pair<string, string>> Assembler::symbolTable;

Assembler::Assembler() : positionCounter(0), lineCounter(0) {
}

string Assembler::process() {
	firstPass();
	return secondPass();
}

string passParametersMacro(string macroLine, string macroDeclaration, string arg1, string arg2) {
	string replace1, replace2;
	
	for (int i = 0; i < macroDeclaration.length(); i++) {
		if (macroDeclaration[i] == '&') {
			if (replace1.empty()) {
				while (++i < macroDeclaration.length() && isalnum(macroDeclaration[i])) {
					replace1 += macroDeclaration[i];
				}
			} else {
				while (++i < macroDeclaration.length() && isalnum(macroDeclaration[i])) {
					replace2 += macroDeclaration[i];
				}
			}
		}
	}

	while (macroLine.find('&' + replace1) != string::npos) {
		macroLine.replace(macroLine.find('&' + replace1), replace1.length()+1, arg1);
	}

	while (macroLine.find('&' + replace2) != string::npos) {
		macroLine.replace(macroLine.find('&' + replace2), replace2.length()+1, arg2);
	}


	return macroLine;
}

bool isSymbolInvalid(string symbol) {
	for (int i = 0; i < symbol.size(); i++)
		if (symbol[i] != '_' && !isalpha(symbol[i]) && !isdigit(symbol[i])) return true;
	return false;
}


void Assembler::fillMacro(vector<string> &macro) {
	Source& src = Source::GetInstance();

	string line;

	while (lineCounter < src.getQtdLines()) {
		line = src.readLine(lineCounter);
		Instruction instr(line);
		TO_UPPER(instr.operation);
		if (instr.operation == "ENDMACRO") {
			src.removeLine(lineCounter);
			break;
		}

		macro.push_back(line);
		src.removeLine(lineCounter);
	}
}

void Assembler::preProcess() {
	Source& src = Source::GetInstance();

	string line;
	vector<string> macro1;
	string macro1Name;
	string macro1Declaration;
	vector<string> macro2;
	string macro2Name;
	string macro2Declaration;
	string pendingLabel;
	int pendingLabelIdx = -1;
	for (; lineCounter < src.getQtdLines(); lineCounter++) {
		line = src.readLine(lineCounter);
		Instruction instr(line);
		TO_UPPER(instr.label);
		TO_UPPER(instr.operation);
		
		// apenas rotulo
		if (!instr.label.empty() && instr.operation.empty() && instr.op1.empty() && instr.op2.empty() && instr.op3.empty() && instr.arit1.empty() && instr.arit2.empty()) {
			pendingLabelIdx = lineCounter;
			pendingLabel = instr.label;
			continue;
		}

		if (instr.operation == "MACRO") {
			if (!pendingLabel.empty()) {
				instr.label = pendingLabel + (instr.label.empty() ? "" : (':' + instr.label));
				line = instr.label + ": " + line;
				src.removeLine(pendingLabelIdx);
				lineCounter--;
			}
			if (macro1.empty()) {
				macro1Name = instr.label;
				macro1Declaration = line;
				src.removeLine(lineCounter);
				fillMacro(macro1);
			} else {
				macro2Name = instr.label;
				macro2Declaration = line;
				src.removeLine(lineCounter);
				fillMacro(macro2);
			}
			line = src.readLine(lineCounter);
			instr = Instruction(line);
			TO_UPPER(instr.label);
			TO_UPPER(instr.operation);
		}

		if (!instr.operation.empty() && instr.operation == macro1Name) {
			src.removeLine(lineCounter);
			for (int i = 0; i < macro1.size(); i++) {
				src.insertLine(lineCounter, passParametersMacro(macro1[i], macro1Declaration, instr.op1, instr.op2));
				lineCounter++;
			}
		} else if (!instr.operation.empty() && instr.operation == macro2Name) {
			src.removeLine(lineCounter);
			for (int i = 0; i < macro2.size(); i++) {
				src.insertLine(lineCounter, passParametersMacro(macro2[i], macro2Declaration, instr.op1, instr.op2));
				lineCounter++;
			}
			lineCounter -= macro2.size()+1;	// no caso de chamar outra macro
		}

		if (!instr.label.empty() || !instr.operation.empty() || !instr.op1.empty() || !instr.op2.empty() || !instr.op3.empty() || !instr.arit1.empty() && !instr.arit2.empty()) {
			pendingLabelIdx = -1;
			pendingLabel = "";
		}

	}

	lineCounter = 0;
	ofstream preprocessedFile(src.getFileName().substr(0, src.getFileName().size()-4) + ".pre");
    if(preprocessedFile.is_open()) {
        string line;
        do {
			line = src.readLine(lineCounter++);
			if (!line.empty() && line != END_OF_FILE) {
            	preprocessedFile << line << endl;
			}
			if (line == END_OF_FILE) break;
        } while (true);
        preprocessedFile.close();
    } else {
		cout << "Não foi possivel criar o arquivo pré-processado" << endl;
	}
	lineCounter = 0;
}

void Assembler::firstPass() {
	Source& src = Source::GetInstance();
	string fileName = src.getFileName();

	if (fileName.substr(fileName.size()-4, fileName.size()) == ".mcr") {
		preProcess();
	}

	string pendingLabel;
	string line;
	for (; lineCounter < src.getQtdLines(); lineCounter++) {
		line = src.readLine(lineCounter);
		Instruction instr(line);
		TO_UPPER(instr.label);
		TO_UPPER(instr.operation);
		TO_UPPER(instr.op1);
		TO_UPPER(instr.op2);

		if (!pendingLabel.empty()) {
			instr.label = pendingLabel + (instr.label.empty() ? "" : (':' + instr.label));
		}

		// se existe rotulo, pesquisar na tabela de simbolos
		if (!instr.label.empty()) {
			bool invalid = false;
			if (isdigit(instr.label[0])) invalid = true;
			else 
				invalid = isSymbolInvalid(instr.label);

			if (invalid) {
				Error::print(Error::LEXICAL, lineCounter+1, "Rótulo '" + instr.label + "' inválido", line);
			} else {
				if (pendingLabel.empty()) {
					int idx = searchLabel(instr.label);
					if (idx >= 0) {
						Error::print(Error::SEMANTIC, lineCounter+1, "Rótulo '" + instr.label + "' redefinido", line);
					} else {
						insertLabel(instr.label);
					}
				}
			}
			if (!line.empty()) {
				pendingLabel = "";
			}

		}

		// Procura operacao na tabela de instruções
		if (instructionTable.find(instr.operation) != instructionTable.end()) {
			if (instr.operation == "COPY") {
				positionCounter += 3;
			} else if (instr.operation == "STOP") {
				positionCounter += 1;
			} else {
				positionCounter += 2;
			}
		} else {
			// procurar na tabela de diretivas
			if (instr.operation == "SPACE") {
				if (instr.op1.empty()) {
					positionCounter += 1;
				} else if (isdigit(instr.op1[0]) || instr.op1[0] == '-') {
					int size = stoi(instr.op1);
					if (size < 1) {
						positionCounter += 1;
					} else {
						positionCounter += size;
					}
				}
			} else if (instr.operation == "CONST") {
				if (instr.op1 != "" && isdigit(instr.op1[0]))
					positionCounter += 1;
			} else {
				if (instr.label.empty() && instr.operation.empty() && instr.op1.empty() && instr.op2.empty() && instr.op3.empty()) { // linha em branco
				} else if (!instr.label.empty() && instr.operation.empty() && instr.op1.empty() && instr.op2.empty() && instr.op3.empty()) { // apenas rotulo
					pendingLabel = instr.label;
				} else
					Error::print(Error::LEXICAL, lineCounter+1, "operacao '" + instr.operation + "' inválida", line);
			}
		}
	}
}


string Assembler::secondPass() {
	Source& src = Source::GetInstance();

	string objCode = "";

	positionCounter = 0;
	lineCounter = 0;

	string line;
	for (; lineCounter < src.getQtdLines(); lineCounter++) {
		line = src.readLine(lineCounter);
		Instruction instr(line);
		TO_UPPER(instr.label);
		TO_UPPER(instr.operation);
		TO_UPPER(instr.op1);
		TO_UPPER(instr.op2);

		if (instructionTable.find(instr.operation) != instructionTable.end()) {
			if (instr.operation == "COPY") {
				if (instr.op1.empty() || instr.op2.empty() || !instr.op3.empty())
					Error::print(Error::SYNTACTIC, lineCounter+1, "'COPY' requer 2 operandos", line);
				positionCounter += 3;
			} else if (instr.operation == "STOP") {
				if (instr.op1 != "")
					Error::print(Error::SYNTACTIC, lineCounter+1, "'STOP' não requer operandos", line);
				positionCounter += 1;
			} else {
				if (instr.op1 == "" || instr.op2 != "" || instr.op2 != "")
					Error::print(Error::SYNTACTIC, lineCounter+1, "'" + instr.operation + "'" + " requer 1 operando", line);
				positionCounter += 2;
			}
			objCode += instructionTable[instr.operation] + " ";
		} else {
			if (instr.operation == "SPACE") {
				if (instr.op1.empty()) {
					positionCounter += 1;
					objCode += "0 ";
				}
				else {
					bool invalidArgument = false;
					CHECK_NUMBER(instr.op1, invalidArgument);
					
					if (!invalidArgument) {
						int size = stoi(instr.op1);
						if (size < 1) {
							Error::print(Error::SYNTACTIC, lineCounter+1, "diretiva '" + instr.operation + "' requer um argumento maior que 0", line);
							positionCounter += 1;
						} else {
							positionCounter += size;
							while (size--)	objCode += "0 ";
						}
					} else {
						Error::print(Error::SYNTACTIC, lineCounter+1, "argumento inválido para diretiva '" + instr.operation + "'", line);
					}
				}
			} else if (instr.operation == "CONST") {
					bool invalidArgument = false;
					if (!instr.op1.empty()) {
						if (instr.op1[0] == '-' && instr.op1.size() > 1) {
							string negativeNumber = instr.op1.substr(1, instr.op1.size()-1);
							CHECK_NUMBER(negativeNumber, invalidArgument);
						} else {
							CHECK_NUMBER(instr.op1, invalidArgument);
						}

						positionCounter += 1;
						objCode += to_string(stoi(instr.op1)) + " ";
					} else if (!isdigit(instr.op1[0])) {
						Error::print(Error::SYNTACTIC, lineCounter+1, "argumento inválido para diretiva '" + instr.operation + "'", line);
					}
			}
		}


		if (instr.op1 != "") {
			bool invalidArit1 = false;
			int sum = 0;
			if (!instr.arit1.empty()) {
				string afterOperation = instr.arit1.substr(1, instr.arit1.size()-1);
				CHECK_NUMBER(afterOperation, invalidArit1);
				if (invalidArit1) {
					Error::print(Error::LEXICAL, lineCounter+1, "expressão aritmética inválida", line);
				} else {
					sum = stoi(instr.arit1);
				}
			}

			if (!invalidArit1 && instr.operation != "CONST" && instr.operation != "STOP" && !isdigit(instr.op1[0])) {
				int position = searchLabel(instr.op1);
				bool invalid;
				if (position < 0) {
					invalid = isSymbolInvalid(instr.op1);
					if (!invalid)
						Error::print(Error::SEMANTIC, lineCounter+1, "símbolo '" + instr.op1 +  "' não definido", line);
					else
						Error::print(Error::LEXICAL, lineCounter+1, "símbolo '" + instr.op1 +  "' inválido", line);
				} else {
					objCode += to_string(stoi(symbolTable[position].second) + sum) + " ";
				}
			}
		}

		if (instr.op2 != "") {
			bool invalidArit2 = false;
			int sum = 0;
			if (!instr.arit2.empty()) {
				string afterOperation = instr.arit2.substr(1, instr.arit2.size()-1);
				CHECK_NUMBER(afterOperation, invalidArit2);
				if (invalidArit2) {
					Error::print(Error::LEXICAL, lineCounter+1, "expressão aritmética inválida", line);
				} else {
					sum = stoi(instr.arit2);
				}
			}

			if (!invalidArit2 && instr.operation == "COPY" && !isdigit(instr.op2[0])) {
				int position = searchLabel(instr.op2);
				bool invalid;
				if (position < 0) {
					invalid = isSymbolInvalid(instr.op2);
					if (!invalid)
						Error::print(Error::SEMANTIC, lineCounter+1, "símbolo '" + instr.op2 +  "' não definido", line);
					else
						Error::print(Error::LEXICAL, lineCounter+1, "símbolo '" + instr.op2 +  "' inválido", line);
				} else {
					objCode += to_string(stoi(symbolTable[position].second)+sum) + " ";
				}
			} else {
				Error::print(Error::LEXICAL, lineCounter+1, "símbolo '" + instr.op2 +  "' inválido", line);
			}
		}
	}

	LOG_D(objCode);

	return objCode;

}


int Assembler::searchLabel(string label) {
	for (int i = 0; i < symbolTable.size(); i++) {
		if (symbolTable[i].first == label) {
			return i;
		}
	}
	return -1;
}

void Assembler::insertLabel(string label) {
	symbolTable.push_back({label, to_string(positionCounter)});
}
