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

bool typeCheck(vector<ParseToken> &symbolTable, string symbol, string type) {
	string foundType = "";
	for (int i = 0; i < symbolTable.size(); i++) {
		if (symbol == symbolTable[i].symbol) {
			if (type == symbolTable[i].type)
			{
				return true;
			}
			foundType = symbolTable[i].type;
		}
	}
	cout << "Semantics Error: Symbol of type " << foundType << " found.  Expected type " << type << endl;
	exit(1);
}
