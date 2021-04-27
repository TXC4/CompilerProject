#pragma once
#include "LexUtils.h"

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>

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

string readFile(string fileName) {
	ifstream inFile;
	string returnString = "";
	inFile.open(fileName, ios::in);
	string newString = "";
	if (inFile.is_open()) {
		while (getline(inFile, newString)) {
			returnString.append(newString);
			returnString.append("\n");
		}
	}
	return returnString;
}

// operators
vector<string>operators = { "+", "-", "*", "/", "=", ">", "<", ">=", "<=", "whilePush", "whilePop", "printLabel", "data", "bss" };
int getOperatorIndex(string strData) {
	for (int i = 0; i < operators.size(); i++) {
		if (operators[i] == strData) {
			return i;
		}
	}
	cout << "Error, Operator not found" << endl;
	return -1;
}

string removeBrackets(string str) {
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == '[' || str[i] == ']') {
			str.erase(i, 1);
		}
	}
	return str;
}

void generateCode() {
	//clear GeneratedAssembly.txt
	ofstream out;
	out.open("Resources/GeneratedAssembly.txt", ofstream::out | ofstream::trunc);
	out.close();

	writeAssembly(readFile("Resources/StandardLibrary.txt"));
	writeAssembly("section .bss\n");
	writeAssembly("T0 resw 1\nT1 resw 1\nT2 resw 1\nT3 resw 1\nT4 resw 1\n\n");
	writeAssembly("section .text \n_start:\n");
	
	string sectionState = "text";
	writeAssembly("section .text\n");
	vector<string> quads = readQuads();



	cout << "Printing quads: " << endl;
	for (int i = 0; i < quads.size(); i++) {
		cout << quads[i] << endl;;
	}
	for (int i = 0; i < quads.size(); i++) {
		if (!quads[i].empty() && quads[i][0] != '\n') {
			vector<string> quarters;
			string temp = "";
			//separate a quad string into 4 parts
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

			//make sure writing to correct section
			//verify in .text
			if (quarters[0] != "bss" && quarters[0] != "data") {
				if (sectionState != "text")
				{
					sectionState = "text";
					writeAssembly("section .text\n");
				}
			}
			//verify in .bss
			else if (quarters[0] == "bss") {
				if (sectionState != "bss") {
					if (sectionState == "text")
						writeAssembly("mov ax, 1\nint 0x80\n");
					sectionState = "bss";
					writeAssembly("section .bss\n");
				}
			}
			//verify in .data
			else if (quarters[0] == "data") {
				if (sectionState != "data") {
					if (sectionState == "text")
						writeAssembly("mov ax, 1\nint 0x80\n");
					sectionState = "data";
					writeAssembly("section .data\n");
				}
			}

			// literal or not?
			if (sectionState == "text") {
				if (!isDigit(quarters[1][0])) {
					quarters[1] = "[" + quarters[1] + "]";
				}
				if (!isDigit(quarters[2][0])) {
					quarters[2] = "[" + quarters[2] + "]";
				}
				if (!isDigit(quarters[3][0])) {
					quarters[3] = "[" + quarters[3] + "]";
				}
			}

			
			//instruction blocks
			string instructions = "";
			int quadCode = getOperatorIndex(quarters[0]);
			switch (quadCode) {
			case 0: // +
				instructions =
					"mov ax, " + quarters[1] + "\n" +
					"add ax, " + quarters[2] + "\n" +
					"mov " + quarters[3] + ", ax\n";
				break;
			case 1: // -
				instructions =
					"mov ax, " + quarters[1] + "\n" +
					"sub ax, " + quarters[2] + "\n" +
					"mov " + quarters[3] + ", ax\n";
				break;
			case 2: // *
				instructions =
					"mov ax, " + quarters[1] + "\n" +
					"mov bx, " + quarters[2] + "\n" +
					"mul bx\n" +
					"mov " + quarters[3] + ", ax\n";
				break;
			case 3: // /
				instructions =
					"mov dx, 0\nmov ax, " + quarters[1] + "\n" +
					"mov bx, " + quarters[2] + "\n" +
					"idiv bx";
				break;
			case 4: // =
				instructions =
					"mov ax, " + quarters[2] + "\n" +
					"mov " + quarters[1] + ", word ax\n";
				break;
			case 5: // >
				quarters[3] = removeBrackets(quarters[3]);
				instructions =
					"mov ax, " + quarters[1] + "\n" +
					"cmp ax, " + quarters[2] + "\n" +
					"jle " + quarters[3] + "\n";
				break;
			case 6: // <
				quarters[3] = removeBrackets(quarters[3]);
				instructions =
					"mov ax, " + quarters[1] + "\n" +
					"cmp ax, " + quarters[2] + "\n" +
					"jge " + quarters[3] + "\n";
				break;
			case 7: // >=
				quarters[3] = removeBrackets(quarters[3]);
				instructions =
					"mov ax, " + quarters[1] + "\n" +
					"cmp ax, " + quarters[2] + "\n" +
					"jl " + quarters[3] + "\n";
				break;
			case 8: // <=
				quarters[3] = removeBrackets(quarters[3]);
				instructions =
					"mov ax, " + quarters[1] + "\n" +
					"cmp ax, " + quarters[2] + "\n" +
					"jg " + quarters[3] + "\n";
				break;
			case 9: // whilePush
				quarters[1] = removeBrackets(quarters[1]);
				instructions =
					quarters[1] + ":\n";
				break;
			case 10: // whilePop
				quarters[1] = removeBrackets(quarters[1]);
				quarters[2] = removeBrackets(quarters[2]);
				instructions =
					"jmp " + quarters[1] + "\n" +
					quarters[2] + ": NOP\n";
				break;
					
			case 11: // printInt
				instructions =
					"mov ax, " + quarters[1] + "\n" +
					"call convertIntToStr\n" +
					"call write";
				break;
			case 12: // data
				instructions = quarters[2] + ": dw " + quarters[3] + "\n";
				break;
			case 13: // bss
				instructions = quarters[2] + " " + quarters[1] + " 1\n";
				break;
			}
			writeAssembly(instructions);
		}
	}
}