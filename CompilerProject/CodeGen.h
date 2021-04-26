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

	writeAssembly("global _start\n_start:\n");
	writeAssembly("section .bss\n");
	writeAssembly("T0 resw 1\nT1 resw 1\nT2 resw 1\nT3 resw 1\nT4 resw 1\n\n");
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
				if (!isDigit(quarters[1][0]) && quarters[1][0] != 'r') {
					quarters[1] = "[" + quarters[1] + "]";
				}
				if (!isDigit(quarters[2][0]) && quarters[2][0] != 'r') {
					quarters[2] = "[" + quarters[2] + "]";
				}
				if (!isDigit(quarters[3][0])) {
					quarters[3] = "[" + quarters[3] + "]";
				}
			}
			
			//instruction blocks
			string instructions = "";
			switch (quads[i][0]) {
			case '+':
				instructions =
					"mov ax, " + quarters[1] + "\n" +
					"add ax, " + quarters[2] + "\n" +
					"mov " + quarters[3] + ", ax\n";
				break;
			case '-':
				instructions =
					"mov ax, " + quarters[1] + "\n" +
					"sub ax, " + quarters[2] + "\n" +
					"mov " + quarters[3] + ", ax\n";
				break;
			case '*':
				instructions =
					/*"mov ax, [" + quarters[1] + "]\n" +
					"mul [" + quarters[2] + "]\n" +
					"mov [T1], ax\n";*/
					"mov ax, " + quarters[1] + "\n" +
					"mov bx, " + quarters[2] + "\n" +
					"mul bx\n" +
					"mov " + quarters[3] + ", ax\n";
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
					"mov " + quarters[1] + ", word ax\n";
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
			case 'd':
				instructions = quarters[2] + ": dw " + quarters[3] + "\n";
				break;
			case 'b':
				instructions = quarters[2] + " " + quarters[1] + " 1\n";
				break;
			}
			writeAssembly(instructions);
		}
	}
}