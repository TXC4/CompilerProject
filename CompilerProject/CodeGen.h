#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void generateCode() {
	string quad = "+, A, B, T1";
	vector<string> quarters;
	for (int i = 0; i < quad.size(); i++) {
		if (quad[i] != ' ' && quad[i] != ',') {
			string temp = "";
			temp.push_back(quad[i]);
			quarters.push_back(temp);
		}
	}

	string instructions = "";
	switch (quad[0]) {
	case '+':
		instructions =
			"mov ax, [" + quarters[1] + "]\n" +
			"add ax, [" + quarters[2] + "]\n" +
			"mov [T1], ax\n";
		break;
	case '-':
		instructions =
			"mov ax, [" + quarters[1] + "]\n" +
			"sub ax, [" + quarters[2] + "]\n" +
			"mov [T1], ax\n";
		break;
	case '*':
		instructions =
			"mov ax, [" + quarters[1] + "]\n" +
			"mul [" + quarters[2] + "]\n" +
			"mov [T1], ax\n";
		break;
	case '/':
		instructions =
			"mov dx, 0\nmov ax, [" + quarters[1] + "]\n" +
			"mov bx, [" + quarters[2] + "]\n" +
			"idiv bx";
		break;
	case '=':
		instructions =
			"mov ax, [" + quarters[2] + "]\n" +
			"mov [" + quarters[1] + "]\n";
		break;
	case '>':
		instructions =
			"mov ax [" + quarters[1] + "]\n" +
			"cmp ax, [" + quarters[2] + "]\n" +
			"jle label1\n";
	case '<':
		instructions =
			"mov ax [" + quarters[1] + "]\n" +
			"cmp ax, [" + quarters[2] + "]\n" +
			"jge label1\n";
	}
	
	ofstream fileObj;
	fileObj.open("Resources/instructions.txt");
	fileObj << instructions;
	fileObj.close();
}