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
		vector<string> biOpList = { "+", "-", "*", "/" };
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

	int operations(vector<string>& thisPop) {
		string opType = "";
		int t1 = -2;
		bool tb1 = false;
		for (int i = 0; i < thisPop.size(); i++) {
			if (isArithmeticOp(thisPop[i])) {
				// check if thisPop[0] or thisPop[2] is var or const
				// if so we will search the symbol table for it
				// then get the value at that location
				if (isLetter(thisPop[0][0])) {

				}
				cout << "Arithmetic Operation: " << thisPop[2] << thisPop[1] << thisPop[0] << endl;
				switch (thisPop[i][0]) {
				case '+':
					t1 = atoi(thisPop[2].c_str()) + atoi(thisPop[0].c_str());
					break;
				case '*':
					t1 = atoi(thisPop[2].c_str()) * atoi(thisPop[0].c_str());
					break;
				case '-':
					t1 = atoi(thisPop[2].c_str()) - atoi(thisPop[0].c_str());
					break;
				case '/':
					t1 = atoi(thisPop[2].c_str()) / atoi(thisPop[0].c_str());
					break;
				case '=':
					int temp;
					temp = atoi(thisPop[2].c_str());
					thisPop[0] = to_string(temp);
					break;
				case '>':
					tb1 = thisPop[0] > thisPop[2];
					break;
				default:
					t1 = -1;
					break;
				}

				cout << "t1 = " << t1 << endl;
				return t1;
			}
			else if (isRelationalOp(thisPop[i])) {
				cout << "Relational Operation: " << thisPop[2] << thisPop[1] << thisPop[0] << endl;
				switch (thisPop[i][0]) {
				case '>':
					tb1 = thisPop[2] > thisPop[0];
					break;
				}
				cout << "tb1 = " << tb1 << endl;
				return tb1;
			}
			else if (thisPop[i] == ")") {
				cout << "Operation: Popped parenthesis, left with '" << thisPop[1] << endl;
				t1 = atoi(thisPop[1].c_str());
			}
			else if (thisPop[i] == "}") {
				cout << "Operation: Popped curly brackets, left with '" << thisPop[1] << endl;
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
		vector<ParseToken> symbolTable = {};
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