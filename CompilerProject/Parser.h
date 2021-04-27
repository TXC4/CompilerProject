#pragma once

#include "ParserUtils.h"

#include <iostream>
#include <vector>
#include <deque>
#include <sstream>

using namespace std;
using namespace parserUtils;

bool operatorPrecedenceParser(vector<vector<string>> table, vector<string> tableHeader, vector<string> tokenString, vector<ParseToken> &symbolTable) {
	deque<string> tokenStack;
	string lastPushedOp = "";

	//clear Quads.txt
	ofstream out;
	out.open("Resources/Quads.txt", ofstream::out | ofstream::trunc);
	out.close();

	tokenStack.push_back(";");
	lastPushedOp = ";";
	int tempCount = 0;

	for (int i = 0; i < tokenString.size(); i++) {
		if (!parserUtils::isOperator(tokenString[i])) {
			tokenStack.push_back(tokenString[i]);
		}
		else {
			//find where x == lastPushedOp and y == tokenString[i], check precedence relation
			string relation = "";
			int lastOpIndex = 6;
			int nextOpIndex = -1;
			
			for (int j = 0; j < tableHeader.size(); j++) {
				if (lastPushedOp == tableHeader[j]) {
					lastOpIndex = j;
				}
				if (tokenString[i] == tableHeader[j]) {
					nextOpIndex = j;
				}
			}
			//get relation
			relation = table[lastOpIndex][nextOpIndex];
			cout << "Relation " << tableHeader[lastOpIndex] << " " << relation << " " << tableHeader[nextOpIndex] << endl;
			vector<string> thisPop = {};
			// ';' resets tempCount
			if (lastOpIndex == 6)
				tempCount = 0;
			//reduce
			if (relation == ">") {
				if (isArithmeticOp(lastPushedOp) || isRelationalOp(lastPushedOp)) {
					while (tokenStack.back() != lastPushedOp) {
						thisPop.push_back(tokenStack.back());
						tokenStack.pop_back();
					}
					do {
						thisPop.push_back(tokenStack.back());
						tokenStack.pop_back();
					} while (!parserUtils::isOperator(tokenStack.back()));
				}
				else if (lastPushedOp == ")") {
					while (tokenStack.back() != "(") {
						thisPop.push_back(tokenStack.back());
						tokenStack.pop_back();
					}
					thisPop.push_back(tokenStack.back());
					tokenStack.pop_back();
				}
				else if (lastPushedOp == "}") {
					while (tokenStack.back() != "{") {
						thisPop.push_back(tokenStack.back());
						tokenStack.pop_back();
					}
					thisPop.push_back(tokenStack.back());
					tokenStack.pop_back();
				}
				else if (lastPushedOp == "DO" && tableHeader[nextOpIndex] == ";") {
					while (tokenStack.back() != "WHILE") {
						thisPop.push_back(tokenStack.back());
						tokenStack.pop_back();
					}
					thisPop.push_back(tokenStack.back());
					tokenStack.pop_back();
				}

				lastPushedOp = tokenStack.back();
				if (thisPop[1] == "=" || thisPop[0] == "DO") {
					operations(thisPop, symbolTable, tempCount);
				}
				else {
					//UNCOMMENT AND IN OPERATIONS() RETURN ACTUAL INT VALUE
					//tokenStack.push_back(to_string(operations(thisPop, symbolTable, tempCount)));
					tokenStack.push_back(operations(thisPop, symbolTable, tempCount));
					tempCount++;
				}
				
				
				i--;
			}
			else if (relation == "<" || relation == "=") {
				tokenStack.push_back(tokenString[i]);
				lastPushedOp = tokenString[i];
				// "WHILE" pushed to stack
				if (tokenString[i] == "WHILE") {
					string whileLabel = pushLabel("whileLabelStack");
					string generalLabel = pushLabel("labelStack");
					string quadLine = "whilePush," + whileLabel + "," + generalLabel + ",~";
					writeQuads(quadLine);
				}
			}
			else {
				cout << "No relation\n";
			}
			cout << endl;
		}
		//print stack
		for (int j = 0; j < tokenStack.size(); j++) {
			cout << tokenStack[j] << endl;
		}
		cout << endl;
	}
	return true;
}

void parse() {
	vector<ParseToken> symbolTable = readSymbolTable();

	vector<string> completeTableHeader = { "+", "-", "*", "/", "(", ")", ";", "{", "}", ">", ">=", "WHILE", "=", "DO" };
	vector<vector<string>> completeTable = {
		{">",">","<","<","<",">",">", "",">", "", "", "", "", ""},
		{">",">","<","<","<",">",">", "",">", "", "", "", "", ""},
		{">",">",">",">","<",">",">", "",">", "", "", "", "", ""},
		{">",">",">",">","<",">",">", "",">", "", "", "", "", ""},
		{"<","<","<","<","<","=", "", "", "","<","<", "", "", ""},
		{">",">",">",">", "",">",">", "", "", "", "", "", "", ""},
		{"<","<","<","<","<", "", "", "", "", "", "","<","<", ""},
		{"<","<","<","<","<", "", "",">","=", "", "","<","<", ""},
		{ "", "", "", "", "", "",">", "",">", "", "","<", "", ""},
		{"<","<","<","<","<",">", "", "", "", "", "", "", "",">"},
		{"<","<","<","<","<",">", "", "", "", "", "", "", "",">"},
		{"<","<","<","<","<", "",">", "", "","<","<", "", "","="},
		{"<","<","<","<","<", "",">", "", "","<", "", "", "", ""},
		{ "", "", "", "","<", "",">", "", "", "","<", "","<", ""}
	};

	vector<string> tokenString = readFromLex();
	operatorPrecedenceParser(completeTable, completeTableHeader, tokenString, symbolTable);
	assemblyInitialization(symbolTable);
}