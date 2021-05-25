//Garrison Travis

#include "KBgeneration.h"

//Function to generate the Australia Map Coloring Knowledge Base
void generateMapColorKB(string filename) {

	string colors[3] = { "R", "G", "B" };
	string states[7] = { "NSW", "NT", "Q", "SA", "T", "V", "WA" };

	//Lists neighbors of the states array
	vector<vector <string>> map = {
		{"Q", "SA", "V"},
		{"Q", "SA", "WA"},
		{"NSW", "NT", "SA"},
		{"NSW", "NT", "Q", "V", "WA"},
		{},
		{"NSW", "SA"},
		{"NT", "SA"}
	};

	ofstream output(filename);

	output << "#Each state has a RGB color" << endl;
	for (int i = 0; i < 7; i++) {
		output << "(or " << states[i] << "R " << states[i] << "G " << states[i] << "B)" << endl;
	}

	output << "\n#Each state has at most one color" << endl;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 3; j++) {
			if (j == 0) {
				output << "(or (not " << states[i] << colors[j] << ") (not " << states[i] << colors[1] << "))" << endl;
				output << "(or (not " << states[i] << colors[j] << ") (not " << states[i] << colors[2] << "))" << endl;
			}
			else if (j == 1) {
				output << "(or (not " << states[i] << colors[j] << ") (not " << states[i] << colors[2] << "))" << endl;
				output << "(or (not " << states[i] << colors[j] << ") (not " << states[i] << colors[0] << "))" << endl;
			}
			else if (j == 2) {
				output << "(or (not " << states[i] << colors[j] << ") (not " << states[i] << colors[0] << "))" << endl;
				output << "(or (not " << states[i] << colors[j] << ") (not " << states[i] << colors[1] << "))" << endl;
			}
		}
	}

	output << "\n#Adjacent states must be different colors" << endl;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < map.at(i).size(); j++) {
			for (int k = 0; k < 3; k++) {
				output << "(or (not " << states[i] << colors[k] << ") (not " << map.at(i).at(j) << colors[k] << "))" << endl;
			}
		}
	}

	output.close();
}

//Function to generate the N-Queens Knowledge Base
void generateNQueensKB(string filename, int n) {
	ofstream output(filename);

	string rule;

	output << "#At least one Queen per row" << endl;
	for (int i = 1; i <= n; i++) {
		rule = "(or ";
		for (int j = 1; j <= n; j++) {
			rule += "Q" + to_string(j) + to_string(i) + " ";
		}
		rule.pop_back();
		rule += ")";
		output << rule << endl;
	}
	output << endl;

	output << "\n#At least one Queen per column" << endl;
	for (int i = 1; i <= n; i++) {
		rule = "(or ";
		for (int j = 1; j <= n; j++) {
			rule += "Q" + to_string(i) + to_string(j) + " ";
		}
		rule.pop_back();
		rule += ")";
		output << rule << endl;
	}
	output << endl;

	output << "\n#Exactly one Queen per column" << endl;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			for (int k = j + 1; k <= n; k++) {
				rule = "(or (not Q" + to_string(i) + to_string(j) + ") (not Q" + to_string(i) + to_string(k) + "))";
				output << rule << endl;
			}
		}
	}

	output << "\n#Exactly one Queen per row" << endl;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			for (int k = j + 1; k <= n; k++) {
				rule = "(or (not Q" + to_string(j) + to_string(i) + ") (not Q" + to_string(k) + to_string(i) + "))";
				output << rule << endl;
			}
		}
	}

	output << "\n#Exactly one Queen per diagonal" << endl;
	for (int i = 1; i <= n; i++) {
		for (int a = 1; a <= n; a++) {
			for (int j = 1; j <= n; j++) {
				for (int b = 1; b <= n; b++) {
					if (abs(i - j) == abs(a - b) && (i != j && a != b)) {
						rule = "(or (not Q" + to_string(i) + to_string(a) + ") (not Q" + to_string(j) + to_string(b) + "))";
						output << rule << endl;
					}
				}
			}
		}
	}
	output << endl;

	output.close();
}