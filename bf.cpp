#include <fstream>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

using std::ifstream;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::stack;
using std::out_of_range;
using std::string;
using std::vector;

int main(int argc, char** argv)
{
	if (argc == 1) {
		cerr << "no file given" << endl;
		return 1;
	}

	ifstream file(argv[1]);
	if (!file) {
		cerr << "invalid or missing filename" << endl;
		return 1;
	}

	// represents the "address field" - a string of cells from 0 to
	// infinity, where each cell stores a number up to 255
	vector<unsigned char> cells(25, 0);
	vector<unsigned char>::size_type index = 0;

	char command;
	while (file.get(command)) {
		switch (command) {
			// move right
			case '>': 
				if (index == cells.max_size()-1) {
					cerr << "program tried to access too large of a cell" << endl;
					return 1;
				}
				++index;
				// add more cells if necessary
				if (index >= cells.size()) {
					cells.push_back(0);
				}
				break;
			// move left
			case '<': 
				if (index == 0) {
					cerr << "program tried to access cell -1" << endl;
					return 1;
				}
				--index;
				break;
			// increment
			case '+': 
				++cells[index];
				break;
			// decrement
			case '-': 
				--cells[index];
				break;
			// write
			case '.': 
				cout << cells[index];
				break;
			// read
			case ',': 
				char c;
				if (!cin.get(c)) {
					c = 0;
				}
				cells[index] = c;
				break;
			// loop begin
			case '[': 
				if (cells[index] == 0) {
					stack<char> brackets;
					brackets.push('[');
					// go until end is reached or all brackets are resolved (a pair is found)
					while (file.get(command)) {
						switch (command) {
							case '[': brackets.push('['); break;
							case ']': brackets.pop(); break;
						}

						if (brackets.size() == 0) {
							break;
						}
					}

					// throw error if any brackets are unresolved
					if (brackets.size() > 0) {
						cerr << ("program had an unclosed '['") << endl;
						return 1;
					}
				}
				break;
			// loop end
			case ']':
				if (cells[index] != 0) {
					stack<char> brackets;
					// see jumpf()
					while (file.unget()) {
						command = file.peek();
						switch (command) {
							case ']': brackets.push(']'); break;
							case '[': brackets.pop(); break;
						}

						if (brackets.size() == 0) {
							break;
						}
					}

					// throw error if any brackets are unresolved
					if (brackets.size() > 0) {
						cerr << "program had an unclosed ']'" << endl;
						return 1;
					}
				}
				break;
		}
	}
	return 0;
}	
