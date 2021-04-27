#pragma once
#include "LexUtils.h"

#include <fstream>
#include <deque>


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

	
	


	vector<string> operatorList = { "+", "-", "*", "/", "(", ")", ";", "{", "}", ">", ">=", "WHILE", "=", "DO", "CALL", "<", "<=" };
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
			if (symbolTable[i].type == "<const>") {
				newStr = "data,=," + symbolTable[i].symbol + "," + to_string(symbolTable[i].value);
			}
			// declare var
			else {
				if (symbolTable[i].type == "<var>") {
					newStr = "bss,resw," + symbolTable[i].symbol + ",~";
				}
			}
			writeQuads(newStr);
		}
	}

	// Label stacks
	deque<string> whileLabelStack;
	deque<string> labelStack;

	string pushLabel(string dequeStack) {
		string label = "";
		if (dequeStack == "whileLabelStack") {
			label = "W" + to_string(whileLabelStack.size());
			whileLabelStack.push_back(label);
		}
		else if (dequeStack == "labelStack") {
			label = "L" + to_string(labelStack.size());
			labelStack.push_back(label);
		}
		return label;
	}
	
	string popLabel(string dequeStack) {
		string label = "";
		if (dequeStack == "whileLabelStack") {
			label = whileLabelStack.back();
			whileLabelStack.pop_back();
		}
		else if (dequeStack == "labelStack") {
			label = labelStack.back();
			labelStack.pop_back();
		}
		return label;
	}

	string operations(vector<string> thisPop, vector<ParseToken> &symbolTable, int tempCount) {
		string opType = "";
		int t1 = -2;
		bool tb1 = false;

		for (int i = 0; i < thisPop.size(); i++) {
			
			string quadLine = "";
			if (isArithmeticOp(thisPop[i])) {
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
				//ignore if not compiling to c++
				
				switch (thisPop[i][0]) {
				case '+':
					t1 = atoi(arg1.c_str()) + atoi(arg2.c_str());
					break;
				case '*':
					t1 = atoi(arg1.c_str()) * atoi(arg2.c_str());
					break;
				case '-':
					t1 = atoi(arg1.c_str()) - atoi(arg2.c_str());
					break;
				case '/':
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
				if (tempStr == "+" || tempStr == "-" || tempStr == "*" || tempStr == "/")
					quadLine = tempStr + "," + thisPop[2] + "," + thisPop[0] + "," + "T" + to_string(tempCount);
				else if (tempStr == "=")
					quadLine = tempStr + "," + thisPop[2] + "," + thisPop[0] + "," + "~";
				writeQuads(quadLine);

				//UNCOMMENT TO TRANSLATE TO C++
				//return t1;

				string returnTemp = "T" + to_string(tempCount);
				return returnTemp;
			}
			else if (isRelationalOp(thisPop[i])) {
				// check if thisPop[0] or thisPop[2] is var or const
				// if so we will search the symbol table for it
				// then get the value
				string arg1 = thisPop[2];
				string arg2 = thisPop[0];

				if (isLetter(arg1[0])) {
					arg1 = to_string(getValue(arg1, symbolTable));
				}
				if (isLetter(arg2[0])) {
					arg2 = to_string(getValue(arg2, symbolTable));
				}

				cout << "Relational Operation: " << thisPop[2] << thisPop[1] << thisPop[0] << endl;
				switch (thisPop[i][0]) {
				case '>':
					tb1 = atoi(arg1.c_str()) > atoi(arg2.c_str());
					break;
				}

				// quad generation
				string tempStr = thisPop[i];
				quadLine = tempStr + "," + thisPop[2] + "," + thisPop[0] + "," + labelStack.back();
				writeQuads(quadLine);


				//UNCOMMENT TO TRANSLATE TO C++
				//return tb1;
				string returnTemp = "T" + to_string(tempCount);
			}
			else if (thisPop[i] == ")") {
				cout << "Operation: Popped parenthesis" << endl;
				t1 = atoi(thisPop[1].c_str());
			}
			else if (thisPop[i] == "}") {
				cout << "Operation: Popped curly brackets" << endl;
				t1 = atoi(thisPop[1].c_str());
			}
			else if (thisPop[i] == "DO") {
				cout << "Operation: Popped while\n";
				quadLine = "whilePop," + popLabel("whileLabelStack") + "," + popLabel("labelStack") + ",~";
				writeQuads(quadLine);
			}
			else if (thisPop[i] == "CALL") {
				cout << "Operation: Popped call to " << thisPop[0] << endl;
				// check if arguments passed
				if (thisPop.size() > 2) {
					quadLine = "call," + thisPop[1] + "," + thisPop[0] + ",~";
				}
				else {
					quadLine = "call," + thisPop[0] + ",~,~";
				}
				writeQuads(quadLine);
			}
		}
		//UNCOMMENT TO TRANSLATE TO C++
		//return t1;
		string returnTemp = "T" + to_string(tempCount);
		return returnTemp;
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