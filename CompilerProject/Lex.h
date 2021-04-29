#pragma once
#include "LexUtils.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string textFile = "Resources/B5.txt";
string csvFile = "Resources/table.xlsx";
string toParseText = "Resources/lexToParse.txt";

vector<string> stateTableColumns =
{ "CONST", "IF", "VAR", "THEN", "PROCEDURE", "WHILE", "CALL", "DO", "ODD", "CLASS", "=", ",", ";",
"+", "-", "*", "/", "(", ")", "<", ">", "{", "}", "==", ">=", "<=", "!=", "/*", "*/",
"<var>", "<int>", "<any>", "EOF" };

vector<vector<int>> stateTable = {
	{-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 2,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 4,10, 8,10,10,10,10,10,10,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,10,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 5,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 7,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 4, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 9,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 8, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,10,12},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,10,12},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
};

struct Token {
	string symbol;
	string type;
	string address;
	int value = -1;
	string segment;
	bool isEmpty = true;

	Token() {};

	Token(string symbolArg, string typeArg, string segmentArg) {
		symbol = symbolArg;
		type = typeArg;
		segment = segmentArg;
		isEmpty = false;
	}
};

//reads characters in while loop and calls table implementation
string readFile() {
	string codeString = "";
	fstream fileStream;
	fileStream.open(textFile, ios::in);
	if (fileStream.is_open()) {
		char newChar;
		while (fileStream >> noskipws >> newChar) {
			if (newChar != '\n' && newChar != '\t')
			{
				cout << newChar;
				codeString.push_back(newChar);
				if (newChar == '>' || newChar == '<')
				{
					char ch = fileStream.peek();
					if (ch == '=') {
						codeString.push_back(ch);
						fileStream.get();
					}
				}
			}
			if (newChar == '\n' && codeString.back() != ';')
				codeString.push_back(' ');
		}
		//~ = EOF
		codeString.push_back('~');
		codeString.push_back(' ');
		cout << '~';
		cout << endl;
		fileStream.close();
	}
	else {
		cout << "Error: Unable to open " << textFile << endl;
	}
	return codeString;
}

vector<string> getPossibleStates(int currentState) {
	vector<string> possibilities;
	for (int i = 1; i < stateTable[0].size(); i++) {
		if (stateTable[currentState][i] != -1) {
			possibilities.push_back(stateTableColumns[i]);
		}
	}
	return possibilities;
}

int indexOf(string item, vector<string> vec) {
	cout << "token: " << item << ", ";
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] == item) {
			cout << "found at: " << i << endl;
			return i;
		}
	}
	cout << "ERROR returned -2\n";
	return -2;
}

int findMatchInTable(int currentState, string token, vector<vector<string>> automataTable) {
	for (int i = 0; i < automataTable[0].size(); i++) {
		if (automataTable[0][i] == token && token != "") {
			return stoi(automataTable[currentState + 1][i]);
		}
	}
}

string litOrVar(int currentState, string token, vector<vector<string>> automataTable) {
	for (int i = 0; i < automataTable[0].size(); i++) {
		if (automataTable[currentState + 1][i] != "" && (automataTable[0][i] == "<var>" || automataTable[0][i] == "<int>" || automataTable[0][i] == "<auto>" )) {
			return automataTable[0][i];
		}
	}
}

//vector<Token> symbolTable = {};
Token symbolTable[1000];
int symbolTableTop = 0;

int pushToSymbolTable(Token newToken) {
	symbolTable[symbolTableTop] = newToken;
	symbolTableTop++;
	return symbolTableTop - 1;
}

int currentState = 0;
string thisToken = "";
int tokenToAssign;
bool writeToParser = false;
vector<string> parserTokens = {};


void analyze() {
	if (thisToken != "") {
		cout << "current state: " << currentState << endl;
		switch (currentState) {
		case 0:
			cout << "index of: " << indexOf(thisToken, stateTableColumns) << endl;
			currentState = stateTable[currentState][indexOf(thisToken, stateTableColumns)]; break;
		case 1: currentState = stateTable[currentState][indexOf("<var>", stateTableColumns)]; break;
		case 2: currentState = stateTable[currentState][indexOf(thisToken, stateTableColumns)]; 
			parserTokens.push_back(thisToken);
			break;
		case 3:
			if (thisToken == "CONST" || thisToken == "VAR") {
				currentState = stateTable[currentState][indexOf(thisToken, stateTableColumns)];
			}
			else if (isLetter(thisToken[0])) {
				//all reserved words lead to state 10 so <var> is just used for simplicity
				currentState = stateTable[currentState][indexOf("<var>", stateTableColumns)];
				writeToParser = true;
			}
			
			break;
		case 4:
			if (isLetter(thisToken[0])) {
				currentState = stateTable[currentState][indexOf("<var>", stateTableColumns)];
				Token t = Token(thisToken, "<const>", "DS");
				tokenToAssign = pushToSymbolTable(t);
			}
			break;
		case 5: 
			currentState = stateTable[currentState][indexOf(thisToken, stateTableColumns)]; break;
		case 6:
			if (isDigit(thisToken[0])) {
				currentState = stateTable[currentState][indexOf("<int>", stateTableColumns)];
				symbolTable[tokenToAssign].value = stoi(thisToken);
			}
			break;
		case 7: currentState = stateTable[currentState][indexOf(thisToken, stateTableColumns)]; break;
		case 8:
			if (isLetter(thisToken[0])) {
				currentState = stateTable[currentState][indexOf("<var>", stateTableColumns)];
				pushToSymbolTable(Token(thisToken, "<var>", "DS"));
			}
			break;
		case 9: currentState = stateTable[currentState][indexOf(thisToken, stateTableColumns)]; break;
		case 10:
			if (isDigit(thisToken[0])) {
				currentState = stateTable[currentState][indexOf("<int>", stateTableColumns)];
				pushToSymbolTable(Token(thisToken, "<int>", "DS"));
			}
			else if (thisToken == "~") {
				currentState = stateTable[currentState][indexOf("EOF", stateTableColumns)];
			}
			else {
				currentState = stateTable[currentState][indexOf("<any>", stateTableColumns)]; break;
			}
			break;
		case 11:
			if (thisToken == "~") {
				currentState = stateTable[currentState][indexOf("EOF", stateTableColumns)];
			}
			else {
				currentState = stateTable[currentState][indexOf("<any>", stateTableColumns)];
			}
			break;
		case 12: 
			cout << "Success Pass 1\n\n"; break;
		}

		// Error checking
		if (currentState == -1)
		{
			cout << "LEXICAL ERROR: received " << thisToken << ", expected ";
			vector<string> possibleStates = getPossibleStates(currentState);
			for (int i = 0; i < possibleStates.size(); i++) {
				cout << possibleStates[i];
				if (i != possibleStates.size() - 1)
					cout << ", ";
			}
			cout << endl;
			exit(1);
		}
	}
	if (writeToParser && (thisToken != "~")) {
		parserTokens.push_back(thisToken);
	}
}

void buildSymbolTable(string sourceCode) {
	//read in characters
	for (int i = 0; i < sourceCode.size(); i++) {
		cout << sourceCode[i];
	}
	for (int i = 0; i < sourceCode.size(); i++) {
		if (sourceCode[i] == ' ' && thisToken != "") {
			cout << thisToken << endl;
			analyze();
			thisToken = "";
		} 
		else if (sourceCode[i] != ' '){
			thisToken.push_back(sourceCode[i]);
		}

		if (isDelimiter(sourceCode[i])) {
			cout << sourceCode[i] << endl;
			analyze();
			thisToken = "";
		}
		
		if (isDelimiter(sourceCode[i + 1]) && thisToken != "") {
			cout << thisToken << endl;
			analyze();
			thisToken = "";
		}
	}

	//push temporaries to symbol table
	Token T0 = Token("T0", "<int>", "DS");
	Token T1 = Token("T1", "<int>", "DS");
	Token T2 = Token("T2", "<int>", "DS");
	Token T3 = Token("T3", "<int>", "DS");
	Token T4 = Token("T4", "<int>", "DS");
	Token T5 = Token("T5", "<int>", "DS");
	Token T6 = Token("T6", "<int>", "DS");
	Token T7 = Token("T7", "<int>", "DS");
	Token T8 = Token("T8", "<int>", "DS");
	pushToSymbolTable(T0);
	pushToSymbolTable(T1);
	pushToSymbolTable(T2);
	pushToSymbolTable(T3);
	pushToSymbolTable(T4);
	pushToSymbolTable(T5);
	pushToSymbolTable(T6);
	pushToSymbolTable(T7);
	pushToSymbolTable(T8);

	//assign address to token object as string and print & write symbol table
	ofstream outFile("Resources/symbolTable.txt");
	for (int i = 0; i < sizeof(symbolTable)/sizeof(Token); i++) {
		if (!symbolTable[i].isEmpty) {
			symbolTable[i].address = to_string((int)&symbolTable[i]);
			cout << symbolTable[i].symbol << "-" << symbolTable[i].type << " - " << symbolTable[i].value << " - " << symbolTable[i].segment << " - " << symbolTable[i].address << endl;
			outFile << symbolTable[i].symbol << "," << symbolTable[i].type << "," << symbolTable[i].value << "," << symbolTable[i].segment << "," << symbolTable[i].address << "\n";
		}
	}
	outFile.close();
}

void lex() {
	buildSymbolTable(readFile());

	cout << "Code to parse: \n";
	for (int i = 0; i < parserTokens.size(); i++) {
		cout << parserTokens[i] << ", ";
	}
	cout << endl;
	writeToParseFile(parserTokens, toParseText);
}