#pragma once
#include "LexUtils.h"

#include <fstream>



namespace parserUtils {

	struct ParseToken {
		string symbol;
		string type;
		string address;
		int value = -1;
		string segment;
		bool isEmpty = true;

		ParseToken() {};

		ParseToken(string symbolArg, string typeArg, string segmentArg) {
			symbol = symbolArg;
			type = typeArg;
			segment = segmentArg;
			isEmpty = false;
		}
	};

	
	


	vector<string> operatorList = { "+", "-", "*", "/", "(", ")", ";", "{", "}", ">", ">=", "while", "=" };
	bool isOperator(string input) {
		for (int i = 0; i < operatorList.size(); i++) {
			if (input == operatorList[i])
				return true;
		}
		return false;
	}

	bool isArithmeticOp(string input) {
		vector<string> biOpList = { "+", "-", "*", "/", "=" };
		for (int i = 0; i < biOpList.size(); i++) {
			if (input == biOpList[i])
				return true;
		}
		return false;
	}

	bool isRelationalOp(string input) {
		vector<string> relationalOpList = { ">", "<", ">=", "<=", "==", "!=" };
		for (int i = 0; i < relationalOpList.size(); i++) {
			if (input == relationalOpList[i])
				return true;
		}
		return false;
	}

	int getValue(string dataName, vector<ParseToken> symbolTable) {
		for (int i = 0; i < symbolTable.size(); i++) {
			if (symbolTable[i].symbol == dataName) {
				cout << "gotten value: " << symbolTable[i].value << endl;
				return symbolTable[i].value;
			}
		}
		return -1;
	}

	void setValue(string dataName, vector<ParseToken> &symbolTable, int newVal) {
		for (int i = 0; i < symbolTable.size(); i++) {
			if (symbolTable[i].symbol == dataName) {
				symbolTable[i].value = newVal;
			}
		}
	}

	void writeQuads(string line) {
		fstream out;
		string fileName = "Resources/Quads.txt";
		out.open(fileName, ios_base::app | ios::in);
		if (out.is_open())
			out << line << "\n";
		else
			cout << "Error opening " << fileName << endl;
		out.close();
	}

	void assemblyInitialization(vector<ParseToken> symbolTable) {
		for (int i = 0; i < symbolTable.size(); i++) {
			// initialize const
			string newStr = "";
			if (symbolTable[i].value != -1) {
				newStr = "data,=," + symbolTable[i].symbol + "," + to_string(symbolTable[i].value);
			}
			// declare var
			else {
				if (symbolTable[i].type == "<var>") {
					newStr = "bss,resb," + symbolTable[i].symbol;
				}
			}
			writeQuads(newStr);
		}
	}

	int operations(vector<string> thisPop, vector<ParseToken> &symbolTable, int tempCount) {
		string opType = "";
		int t1 = -2;
		bool tb1 = false;

		for (int i = 0; i < thisPop.size(); i++) {
			
			
			if (isArithmeticOp(thisPop[i])) {
				cout << "IS ARITHMETIC OP\n";
				// check if thisPop[0] or thisPop[2] is var or const
				// if so we will search the symbol table for it
				// then get the value
				string arg1 = thisPop[2];
				string arg2 = thisPop[0];
				
				if (isLetter(arg1[0]) && thisPop[i][0] != '=') {
					arg1 = to_string(getValue(arg1, symbolTable));
					cout << "arg1: " << arg1 << endl;
				}
				if (isLetter(arg2[0]) && thisPop[i][0] != '=') {
					arg2 = to_string(getValue(arg2, symbolTable));
					cout << "arg2: " << arg2 << endl;
				}

				cout << "Arithmetic Operation: " << thisPop[2] << thisPop[1] << thisPop[0] << endl;
				//compiles to c++ for log then writes out quads for x86
				string quadLine = "";
				switch (thisPop[i][0]) {
				case '+':
					cout << "Which is: " << arg1.c_str() << " + " << atoi(arg2.c_str()) << endl;
					t1 = atoi(arg1.c_str()) + atoi(arg2.c_str());
					break;
				case '*':
					cout << "Which is: " << arg1.c_str() << " * " << atoi(arg2.c_str()) << endl;
					t1 = atoi(arg1.c_str()) * atoi(arg2.c_str());
					break;
				case '-':
					cout << "Which is: " << arg1.c_str() << " - " << atoi(arg2.c_str()) << endl;
					t1 = atoi(arg1.c_str()) - atoi(arg2.c_str());
					break;
				case '/':
					cout << "Which is: " << arg1.c_str() << " / " << atoi(arg2.c_str()) << endl;
					t1 = atoi(arg1.c_str()) / atoi(arg2.c_str());
					break;
				case '=':
					int temp;
					temp = atoi(arg2.c_str());
					setValue(arg1, symbolTable, temp);
					break;
				default:
					t1 = -1;
					break;
				}
				// quad generation
				string tempStr = "";
				tempStr.push_back(thisPop[i][0]);
				quadLine = tempStr + "," + thisPop[2] + "," + thisPop[0] + "," + "T" + to_string(tempCount);
				cout << "QUADLINE: " << quadLine << endl;
				writeQuads(quadLine);

				cout << "t1 = " << t1 << endl;
				return t1;
			}
			else if (isRelationalOp(thisPop[i])) {
				// check if thisPop[0] or thisPop[2] is var or const
				// if so we will search the symbol table for it
				// then get the value
				string arg1 = thisPop[2];
				string arg2 = thisPop[0];

				if (isLetter(arg1[0])) {
					arg1 = to_string(getValue(arg1, symbolTable));
					cout << "arg1: " << arg1 << endl;
				}
				if (isLetter(arg2[0])) {
					arg2 = to_string(getValue(arg2, symbolTable));
					cout << "arg2: " << arg2 << endl;
				}

				cout << "Relational Operation: " << thisPop[2] << thisPop[1] << thisPop[0] << endl;
				switch (thisPop[i][0]) {
				case '>':
					tb1 = atoi(arg1.c_str()) / atoi(arg2.c_str());
					break;
				}
				cout << "tb1 = " << tb1 << endl;
				return tb1;
			}
			else if (thisPop[i] == ")") {
				cout << "Operation: Popped parenthesis, left with " << thisPop[1] << endl;
				t1 = atoi(thisPop[1].c_str());
			}
			else if (thisPop[i] == "}") {
				cout << "Operation: Popped curly brackets, left with " << thisPop[1] << endl;
				t1 = atoi(thisPop[1].c_str());
			}
		}
		return t1;
	}

	vector<string> readFromLex() {
		vector<string> tokenStream = {};
		ifstream inFile;
		inFile.open("Resources/lexToParse.txt", ios::in);
		if (inFile.is_open()) {
			string newString;
			while (getline(inFile, newString)) {
				tokenStream.push_back(newString);
			}
		}
		else {
			cout << "Error, failed to open file from scanner" << endl;
		}
		return tokenStream;
	}

	vector<ParseToken> readSymbolTable() {
		vector<ParseToken> symbolTable;
		ParseToken newToken;
		string thisString = "";
		char thisChar;
		int commaCount = 0;
		ifstream inFile;
		inFile.open("Resources/symbolTable.txt", ios::in);
		if (inFile.is_open()) {
			while (inFile >> noskipws >> thisChar) {
				cout << thisChar;
				if ((thisChar != ',') && (thisChar != '\n')) {
					thisString.push_back(thisChar);
				}
				else if (thisChar == ',' || thisChar == '\n') {
					switch (commaCount) {
					case 0:
						newToken.symbol = thisString;
						break;
					case 1:
						newToken.type = thisString;
						break;
					case 2:
						newToken.value = stoi(thisString);
						break;
					case 3:
						newToken.segment = thisString;
						break;
					case 4:
						newToken.address = thisString;
						break;
					default:
						cout << "No case match reading symbol table\n";
						break;
					}
					thisString = "";
					commaCount++;
				}
				if (thisChar == '\n') {
					commaCount = 0;
					symbolTable.push_back(newToken);
					thisString = "";
				}
			}
		}
		return symbolTable;
	}
	
}