#pragma once

#include "ParserUtils.h"
#include <vector>
#include <string>

using namespace std;

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

bool typeCheck(vector<ParseToken> &symbolTable, string symbol, vector<string> types) {
	string foundType = "";
	bool tokenFound = false;
	for (int i = 0; i < types.size(); i++) {
		for (int j = 0; j < symbolTable.size(); j++) {
			if (symbol == symbolTable[j].symbol) {
				tokenFound = true;
				foundType = symbolTable[j].type;
				if (types[i] == symbolTable[j].type) {
					return 1;
				}
			}
		}
	}
	//since it was not in the symbol table, check if it is a literal
	for (int i = 0; i < types.size(); i++) {
		if (isDigit(symbol[0]) && types[i] == "<int>") {
			return 1;
		}
	}

	if (tokenFound == true) {
		cout << "SEMANTICS ERROR: found " << foundType << " expected ";
		for (int i = 0; i < types.size(); i++) {
			cout << types[i] << ", ";
		}
	}
	else
	{
		cout << "ERROR: Token '" << symbol << "' does not exist";
	}
	cout << "\n\n";
	exit(1);
}
