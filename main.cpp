//Garrison Travis

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <stdlib.h>

using namespace std;


typedef map<string, char> MODEL;

int numDPLLCalls = 0;
int numClausesSat = 0;

//Function to tokenize a string
vector<string> tokenize(string str) {

	vector<string> tokens;

	for (int i = 0; i < str.size(); i++) {
		if (str[i] == '(' || str[i] == ')') {
			string token(1, str[i]);
			tokens.push_back(token);
		}
		else if (str[i] == ' ')
			continue;
		else {
			string token;
			while (str[i] != ' ' && str[i] != '(' && str[i] != ')') {
				token += str[i];
				i++;
			}

			tokens.push_back(token);
			i--;
		}
	}

	return tokens;
}

//Function to perform DPLL algorithm
bool DPLL(vector<string> clauses, MODEL* model, bool unitOp) {
	numDPLLCalls++;

	//Print Model
	cout << "\nModel: ";
	for (auto it = model->begin(); it != model->end(); ++it)
		cout << it->first << "=" << it->second << " ";
	cout << endl;

	bool violated = true;
	bool unknownFound = false;
	numClausesSat = 0;

	//Used for Unit Clause Heuristic
	int unknownCount = 0;
	int unknownIndex = 0;
	bool unitClause = true;
	string unitSymbol = "";
	char unitSymbolAssignment = NULL;

	//For loops to check if any clauses in the KB have been violated
	for (int i = 0; i < clauses.size(); i++) {

		//Tokenize the clause
		vector<string> tokens = tokenize(clauses[i]);

		//Loop through tokens to find symbols
		for (int j = 0; j < tokens.size(); j++) {

			//If token is a symbol in the model
			if (model->find(tokens[j]) != model->end()) {

				//A Symbol in the clause is unknown in the model
				if (model->at(tokens[j]) == '?') {
					unknownFound = true;
					unknownCount++;
					unknownIndex = j;
					violated = false;

					//If unknownCount is 1, then it could be a unit clause, so continue
					if (unknownCount == 1)
						continue;
					//Else if unknownCount is not 1, then it can't be a unit clause, so break
					else {
						unitClause = false;
						break;
					}
				}

				//If j == 0 then the clause must only have a single symbol
				if (j == 0) {

					//Check if clause is satisfied
					if (model->at(tokens[j]) == 'T') {
						numClausesSat++;
						violated = false;
						unitClause = false;
						break;
					}
				}
				//Clause is a list and this Symbol is of the form (not X)
				else if (tokens[j - 1] == "not") {

					//Check if clause is satisfied
					if (model->at(tokens[j]) == 'F') {
						numClausesSat++;
						violated = false;
						unitClause = false;
						break;
					}
				}
				//Clause is a list and this Symbol is of the form (X)
				else {

					//Check if clause is satisfied
					if (model->at(tokens[j]) == 'T') {
						numClausesSat++;
						violated = false;
						unitClause = false;
						break;
					}
				}
			}
		}

		//If clause was violated, then back track
		if (violated) {
			cout << "Num Clauses Satisfied = " << numClausesSat << " out of " << clauses.size() << endl;
			cout << "Back-tracking . . ." << endl;
			return false;
		}

		//Unit Clause found, store symbol and truth assignment for forcing later
		if (unitOp && unitClause && unitSymbol.empty()) {
			unitSymbol = tokens[unknownIndex];
			if (unknownIndex == 0)
				unitSymbolAssignment = 'T';
			else if (tokens[unknownIndex - 1] == "not")
				unitSymbolAssignment = 'F';
			else
				unitSymbolAssignment = 'T';
		}

		violated = true;		//Reset violated
		unitClause = true;		//Reset unitClause
		unknownCount = 0;		//Reset unknownCount
	}

	cout << "Num Clauses Satisfied = " << numClausesSat << " out of " << clauses.size() << endl;

	//Success (every clause in the KB is satisfied)
	if (!unknownFound) {
		cout << "\nSuccess!" << endl;
		return true;
	}
	//Model needs assignments
	else {
		//If Unit Clause was found from above, force value and recurse
		if (!unitSymbol.empty()) {
			if (unitSymbolAssignment == 'T')
				cout << "Forcing " << unitSymbol << " = 1" << endl;
			else if (unitSymbolAssignment == 'F')
				cout << "Forcing " << unitSymbol << " = 0" << endl;
			model->at(unitSymbol) = unitSymbolAssignment;

			if (DPLL(clauses, model, unitOp))
				return true;
			//DPLL returned false (violation found down the tree), so reset symbol to ? and return false
			else {
				model->at(unitSymbol) = '?';
				return false;
			}
		}
		//Else Unit Clause was not found, try 0/1 on next unknown symbol in model
		else {
			//Find first unassigned symbol in the model and assign it 0/1, then recurse
			for (auto it = model->begin(); it != model->end(); ++it) {
				if (it->second == '?') {
					cout << "Trying " << it->first << " = 0" << endl;
					it->second = 'F';

					if (DPLL(clauses, model, unitOp))
						return true;
					else {
						cout << "Trying " << it->first << " = 1" << endl;
						it->second = 'T';

						if (DPLL(clauses, model, unitOp))
							return true;
						//Tried 0 and 1, but both returned false, so reset to ?, and return false
						else {
							it->second = '?';
							return false;
						}
					}
				}
			}
		}
	}
}

int main(int argc, char* argv[]) {

	vector<string> KB;
	MODEL* model;
	bool unitOp = true;

	//Open the knowledge base file
	ifstream input("marbles.cnf");
	if (!input.is_open()) {
		cout << "Error opening file!" << endl;
		exit(1);
	}

	//Input each line of the knowledge base file into a vector
	string line;
	while (getline(input, line)) {
		if (line.size() != 0 && line[0] != '#')
			KB.push_back(line);
	}

	//Initial unknown model
	model = new MODEL({ {"L1R", '?'}, {"L1G",  '?'}, {"L1B", '?'}, {"L2R", '?'}, {"L2G",  '?'}, {"L2B", '?'}, {"L3R", '?'}, {"L3G",  '?'}, {"L3B", '?'},
						{"O1R", '?'}, {"O1G",  '?'}, {"O2R", '?'}, {"O2G",  '?'}, {"O3R", '?'}, {"O3G",  '?'},
						{"C1R", '?'}, {"C1G",  '?'}, {"C1B", '?'}, {"C2R", '?'}, {"C2G",  '?'}, {"C2B", '?'}, {"C3R", '?'}, {"C3G",  '?'}, {"C3B", '?'} });


	//Output initial KB
	for (int i = 0; i < KB.size(); i++) {
		cout << i + 1 << ". " << KB.at(i) << endl;
	}

	//Perform DPLL
	if (!DPLL(KB, model, unitOp)) {
		cout << "Number of DPLL Calls = " << numDPLLCalls << endl;
		cout << "\nUnsatisfiable!" << endl;
	}
	else {
		if (unitOp)
			cout << "Number of DPLL Calls = " << numDPLLCalls << " (WITH unit-clause heuristic)" << endl;
		else
			cout << "Number of DPLL Calls = " << numDPLLCalls << " (WITHOUT unit-clause heuristic)" << endl;
		cout << "Model:" << endl;
		for (auto it = model->begin(); it != model->end(); ++it)
			std::cout << it->first << ": " << it->second << endl;
	}
}