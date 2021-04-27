#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void print2DVector(vector<vector<string>> scanningTable) {
	int spaceCount = 0;
	string spaceString = "";
	for (auto& row : scanningTable) {
		for (string val : row) {
			if (val.size() < 10)
				spaceCount = 10 - val.size();
			else
				spaceCount = 0;
			for (int i = 0; i < spaceCount; i++) {
				spaceString.append(" ");
			}
			std::cout << val << spaceString;
			spaceString = "";
		}
		std::cout << "\n";
	}
}

void writeToParseFile(vector<string> tokenStream, string fileName) {
	ofstream fileStream(fileName);
	for (int i = 0; i < tokenStream.size(); i++) {
		fileStream << tokenStream[i] << "\n";
	}
	fileStream.close();
}

//parses csv file containing table driven lexical analyzer into 2D vector
vector<vector<string>> csvTo2DArray(string csvFile) {
	vector<vector<string>>scanningTable;
	vector<string>tempVec;
	scanningTable.push_back(tempVec);
	fstream fileStream;
	fileStream.open(csvFile, ios::in);
	if (fileStream.is_open()) {
		char newChar;
		int row = 0;
		string token = "";
		while (fileStream >> noskipws >> newChar) {
			if (newChar == ',') {
				scanningTable.at(row).push_back(token);
				token = "";
			}
			else if (newChar == '\n') {
				scanningTable.at(row).push_back(token);
				token = "";
				row++;
				vector<string>newVec;
				scanningTable.push_back(newVec);
			}
			else {
				token += newChar;
			}
		}
		cout << endl;
		fileStream.close();
	}
	else {
		cout << "Error: Unable to open " << csvFile << endl;
	}
	return scanningTable;
}

void matrixPrintString(vector<vector<string>> matrix) {
	for (int i = 0; i < matrix.size() - 1; i++) {
		cout << endl;
		for (int j = 0; j < matrix.size(); j++) {
			cout << matrix[i][j] << ", ";
		}
	}
	cout << endl << endl;
}

bool isKeyword(string str) {
	if (str == "CONST" || str == "IF" || str == "VAR" || str == "THEN" || str == "PROCEDURE" || str == "WHILE" || str == "CALL" || str == "DO" || str == "ODD" || str == "CLASS")
		return true;
	else
		return false;
}

bool isOperator(string str) {
	if (str == "=" || str == "," || str == ";" || str == "+" || str == "-" || str == "*" || str == "/" || str == "(" || str == ")" || str == "<" || str == ">" || str == "{" || str == "}" || str == "==" || str == ">=" || str == "<=" || str == "!=" || str == "/*" || str == "*/" || str == "WHILE" || str == "DO")
		return true;
	else
		return false;
}

bool isDelimiter(char ch) {
	if (ch == ',' || ch == ';' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}') {
		return true;
	}
	else
		return false;
}

bool isDigit(char ch) {
	if (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9')
		return true;
	else
		return false;
}

bool isLetter(char c) {
	if (c >= 65 && c <= 90 || c >= 97 && c <= 122)
		return true;
	else
		return false;
}