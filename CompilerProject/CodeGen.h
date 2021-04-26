#pragma once
#include "LexUtils.h"

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void writeAssembly(string codeBlock) {
	fstream out;
	string fileName = "Resources/GeneratedAssembly.txt";
	out.open(fileName, ios_base::app | ios::in);
	if (out.is_open())
		out << codeBlock << "\n";
	else
		cout << "Error opening " << fileName << endl;
	out.close();
}

vector<string> readQuads() {
	vector<string> quadList = {};
	ifstream inFile;
	inFile.open("Resources/Quads.txt", ios::in);
	if (inFile.is_open()) {
		string newString;
		while (getline(inFile, newString)) {
			quadList.push_back(newString);
		}
	}
	else {
		cout << "Error, failed to open file from scanner" << endl;
	}
	return quadList;
}

void generateCode() {
	//clear GeneratedAssembly.txt
	ofstream out;
	out.open("Resources/GeneratedAssembly.txt", ofstream::out | ofstream::trunc);
	out.close();

	vector<string> quads = readQuads();
	for (int i = 0; i < quads.size(); i++) {
		if (!quads[i].empty() && quads[i][0] != '\n') {
			vector<string> quarters;
			string temp = "";
			for (int j = 0; j < quads[i].size(); j++) {
				if (quads[i][j] != ' ' && quads[i][j] != ',') {
					temp.push_back(quads[i][j]);
					if (j == quads[i].size() - 1) {
						quarters.push_back(temp);
						temp = "";
					}
				}
				else {
					quarters.push_back(temp);
					temp = "";
				}
			}

			// literal or not
			if (!isDigit(quarters[1][0]) && quarters[1][0] != 'r') {
				quarters[1] = "[" + quarters[1] + "]";
			}
			if (!isDigit(quarters[2][0]) && quarters[2][0] != 'r') {
				quarters[2] = "[" + quarters[2] + "]";
			}

			string instructions = "";
			switch (quads[i][0]) {
			case '+':
				instructions =
					"mov ax, " + quarters[1] + "\n" +
					"add ax, " + quarters[2] + "\n" +
					"mov [T1], ax\n";
				break;
			case '-':
				instructions =
					"mov ax, " + quarters[1] + "\n" +
					"sub ax, " + quarters[2] + "\n" +
					"mov [T1], ax\n";
				break;
			case '*':
				instructions =
					/*"mov ax, [" + quarters[1] + "]\n" +
					"mul [" + quarters[2] + "]\n" +
					"mov [T1], ax\n";*/
					"mov ax, " + quarters[1] + "\n" +
					"mov bx, " + quarters[2] + "\n" +
					"mul bx\n" +
					"mov [T1], ax\n";
				break;
			case '/':
				instructions =
					"mov dx, 0\nmov ax, " + quarters[1] + "\n" +
					"mov bx, " + quarters[2] + "\n" +
					"idiv bx";
				break;
			case '=':
				instructions =
					"mov ax, " + quarters[2] + "\n" +
					"mov " + quarters[1] + "\n";
				break;
			case '>':
				instructions =
					"mov ax " + quarters[1] + "\n" +
					"cmp ax, " + quarters[2] + "\n" +
					"jle label1\n";
				break;
			case '<':
				instructions =
					"mov ax " + quarters[1] + "\n" +
					"cmp ax, " + quarters[2] + "\n" +
					"jge label1\n";
				break;
			}
			writeAssembly(instructions);
		}
	}
}