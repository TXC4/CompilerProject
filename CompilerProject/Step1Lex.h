#pragma once
#include "LexUtils.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string textFile = "Resources/Source.txt";
string csvFile = "Resources/table.csv";

//reads characters in while loop and calls table implementation
void readFile() {
	fstream fileStream;
	fileStream.open(textFile, ios::in);
	if (fileStream.is_open()) {
		char newChar;
		while (fileStream >> noskipws >> newChar) {
			cout << newChar;
		}
		cout << endl;
		fileStream.close();
	}
	else {
		cout << "Error: Unable to open " << textFile << endl;
	}
}

void lex() {
	vector<vector<string>> automataTable = csvTo2DArray(csvFile);
	readFile();
}