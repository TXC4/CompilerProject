#pragma once


namespace parserUtils {

	vector<string> operatorList = { "+", "-", "*", "/", "(", ")", ";", "{", "}", ">", ">=", "while", "=" };
	bool isOperator(string input) {
		for (int i = 0; i < operatorList.size(); i++) {
			if (input == operatorList[i])
				return true;
		}
		return false;
	}

	bool isArithmeticOp(string input) {
		vector<string> biOpList = { "+", "-", "*", "/" };
		for (int i = 0; i < biOpList.size(); i++) {
			if (input == biOpList[i])
				return true;
		}
		return false;
	}

	bool isRelationalOp(string input) {
		vector<string> relationalOpList = { ">", "<", ">=", "<=", "==", "!=" };
		for (int i = 0; i < relationalOpList.size(); i++) {
			if (input == relationalOpList[i])
				return true;
		}
		return false;
	}

	int operations(vector<string>& thisPop) {
		string opType = "";
		int t1 = -2;
		bool tb1 = false;
		for (int i = 0; i < thisPop.size(); i++) {
			if (isArithmeticOp(thisPop[i])) {
				cout << "Arithmetic Operation: " << thisPop[2] << thisPop[1] << thisPop[0] << endl;
				switch (thisPop[i][0]) {
				case '+':
					t1 = atoi(thisPop[2].c_str()) + atoi(thisPop[0].c_str());
					break;
				case '*':
					t1 = atoi(thisPop[2].c_str()) * atoi(thisPop[0].c_str());
					break;
				case '-':
					t1 = atoi(thisPop[2].c_str()) - atoi(thisPop[0].c_str());
					break;
				case '/':
					t1 = atoi(thisPop[2].c_str()) / atoi(thisPop[0].c_str());
					break;
				case '=':
					int temp;
					temp = atoi(thisPop[2].c_str());
					thisPop[0] = to_string(temp);
					break;
				case '>':
					tb1 = thisPop[0] > thisPop[2];
					break;
				default:
					t1 = -1;
					break;
				}

				cout << "t1 = " << t1 << endl;
				return t1;
			}
			else if (isRelationalOp(thisPop[i])) {
				cout << "Relational Operation: " << thisPop[2] << thisPop[1] << thisPop[0] << endl;
				switch (thisPop[i][0]) {
				case '>':
					tb1 = thisPop[2] > thisPop[0];
					break;
				}
				cout << "tb1 = " << tb1 << endl;
				return tb1;
			}
			else if (thisPop[i] == ")") {
				cout << "Operation: Popped parenthesis, left with '" << thisPop[1] << endl;
				t1 = atoi(thisPop[1].c_str());
			}
			else if (thisPop[i] == "}") {
				cout << "Operation: Popped curly brackets, left with '" << thisPop[1] << endl;
				t1 = atoi(thisPop[1].c_str());
			}
		}
		return t1;
	}
}