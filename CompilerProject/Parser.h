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

	tokenStack.push_back(";");
	lastPushedOp = ";";
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
			if (relation == ">") {
				cout << ">" << endl;
				//reduce
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

				lastPushedOp = tokenStack.back();
				if (thisPop[1] != "=") {
					tokenStack.push_back(to_string(operations(thisPop, symbolTable)));
				}
				else {
					operations(thisPop, symbolTable);
				}
				i--;
			}
			else if (relation == "<" || relation == "=") {
				cout << "<" << endl;
				tokenStack.push_back(tokenString[i]);
				lastPushedOp = tokenString[i];
			}
			else {
				cout << "No relation\n";
			}
			cout << "Last pushed op: " << lastPushedOp << endl;
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

	vector<string> completeTableHeader = { "+", "-", "*", "/", "(", ")", ";", "{", "}", ">", ">=", "while", "=" };
	vector<vector<string>> completeTable = {
		{">",">","<","<","<",">",">", "",">", "", "", "", ""},
		{">",">","<","<","<",">",">", "",">", "", "", "", ""},
		{">",">",">",">","<",">",">", "",">", "", "", "", ""},
		{">",">",">",">","<",">",">", "",">", "", "", "", ""},
		{"<","<","<","<","<","=", "", "", "","<","<", "", ""},
		{">",">",">",">", "",">",">", "", "", "", "", "", ""},
		{"<","<","<","<","<", "", "", "", "", "", "", "","<"},
		{"<","<","<","<","<", "", "",">","=", "", "","<","<"},
		{ "", "", "", "", "", "",">", "",">", "", "","<", ""},
		{"<","<","<","<","<",">", "", "", "", "", "", "", ""},
		{"<","<","<","<","<",">", "", "", "", "", "", "", ""},
		{ "", "", "","<", "", "",">", "", "", "", "", "", ""},
		{"<","<","<","<","<", "",">", "", "", "", "", "", ""},
	};

	vector<string> tokenString = readFromLex();
	operatorPrecedenceParser(completeTable, completeTableHeader, tokenString, symbolTable);

	
	cout << "SYMBOL TABLE AS READ TO PARSER.H\n";
	for (int i = 0; i < symbolTable.size(); i++) {
		cout << symbolTable[i].symbol << " - " << symbolTable[i].type << " - " << symbolTable[i].value << " - " << symbolTable[i].segment << " - " << symbolTable[i].address << endl;
	}
}