#include "Source.h"

Source* Source::instance = nullptr;
vector<string> Source::code;
vector<string> Source::dataCode;
string Source::fileName = "";
int Source::qtdLines;

Source& Source::GetInstance() {
	if (instance == nullptr) {
		instance = new Source();
	}

	string formatTest = "ROT:	 STORE  	TMP+2     ;comentario      ";
	LOG_D("\ninput: \"" << formatTest << "\", trimmed: \"" << Source::format(formatTest) << "\"");

	return *instance;
}

Source::Source() {
	fstream file;
 	file.open(fileName, ios::in);

	if (file.is_open()) {
		string line;
		LOG_D("Codigo fonte:");

		bool isData = false, isText = false;
		while (getline(file, line)) {
			line = format(line);

			if (line == "SECAO DATA" && isText == false) {
				isData = false;
				getline(file, line);
				while (line != "SECAO TEXT") {
					dataCode.push_back(line);
					getline(file, line);
				}
			}
			if (line == "SECAO TEXT") {
				isText = true;
			}
			if (line == "SECAO DATA" || line == "SECAO TEXT") continue;


			code.push_back(line);
		}

		// colocar secao de dados no final
		for (int i = 0; i < dataCode.size(); i++) {
			code.push_back(dataCode[i]);
		}

		for (int i = 0; i < code.size(); i++) {
			LOG_D(code[i]);
		}

		LOG_D("");
	} else {
		cout << "Erro ao abrir arquivo " << fileName << endl;
		exit(0);
	}

	qtdLines = code.size();
}

void Source::setFile(string file) {
	fileName = file;
}


string Source::readLine(int i) {
	if (i < qtdLines) {
		string line = code[i];
		LOG_D("linha " << i << ": " << line);
		return line;
	} else {
		return END_OF_FILE;
	}
}

void Source::insertLine(int i, string line) {
	if (i >= qtdLines)
		code.push_back(line);
	else
		code.insert(code.begin() + i, line);
	qtdLines++;
}

void Source::removeLine(int i) {
	if (i < qtdLines) {
		code.erase(code.begin() + i);
		qtdLines--;
	}
}

string Source::format(string line) {
	int i, j;

	// remove comment
	for (i = 0; i < line.size(); i++) {
		if (line[i] == ';') {
			line = line.substr(0, i);
			break;
		}
	}

	// trim begining
	i = 0;
	while (line[i] == ' ' || line[i] == '\t') i++;

	// trim end
	j = line.size()-1;
	while (line[j] == ' ' || line[j] == '\t') j--;

	line = line.substr(i, j-i+1);

	// remove unnecessary spaces
	i = 0;
	int idxSpace = -1;
	int spaceSize = 0;
	for (int i = 0; i < line.size(); i++) {
		if (line[i] == '\t') line[i] = ' ';
		if (line[i] != ' ' && idxSpace >= 0) {
			line = line.substr(0, idxSpace+1) + line.substr(idxSpace+1+spaceSize, line.size()-1);
			i = idxSpace + 1;
			spaceSize = 0;
			idxSpace = -1;
		} else if (line[i] == ' ' && idxSpace >= 0) {
			spaceSize++;
		} else if (line[i] == ' ') {
			idxSpace = i;
		} else {
			idxSpace = -1;
		}
	}

	return line;
}

int Source::getQtdLines() {
	return qtdLines;
}

string Source::getFileName() {
	return fileName;
}
