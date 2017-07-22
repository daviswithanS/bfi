// a brainfuck interpreter
#include <fstream>
#include <iostream>
#include <vector>

using std::ifstream;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
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

	// load instructions and clear all non-brainfuck commands
	vector<char> instructions;
	char command;
	while (file.get(command)) {
		if (   command == '>' || command == '<'
	 	    || command == '+' || command == '-'
		    || command == '.' || command == ',' 
		    || command == '[' || command == ']') {
			instructions.push_back(command);
		}
	}
	vector<char>::const_iterator current = instructions.begin();

	// represents the "address field" - a vector of cells from 0 to
	// infinity, where each cell stores a number up to 255
	vector<unsigned char> cells(3000, 0);
	vector<unsigned char>::size_type index = 0;

	while (current != instructions.end()) {
		switch (*current) {
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
		case '<': 
			if (index == 0) {
				cerr << "program tried to access cell -1" << endl;
				return 1;
			}
			--index;
			break;
		case '+': 
			++cells[index];
			break;
		case '-': 
			--cells[index];
			break;
		case '.': 
			cout << cells[index];
			break;
		case ',': 
			char c;
			if (!cin.get(c)) {
				c = 0;
			}
			cells[index] = c;
			break;
		case '[': 
			if (cells[index] == 0) {
				vector<unsigned char>::size_type brackets = 0;

				// stops going forward at eof or when brackets is 0
				while (current != instructions.end()) {
					switch (*current) {
					case '[': ++brackets; break;
					case ']': --brackets; break;
					}


					if (brackets == 0) {
						break;
					}
					++current;
				}

				if (brackets > 0) {
					cerr << ("program had an unclosed '['") << endl;
					return 1;
				}
			}
			break;
		case ']':
			if (cells[index] != 0) {
				vector<unsigned char>::size_type brackets = 0;

				while (current != instructions.begin()) {
					switch (*current) {
					case ']': ++brackets; break;
					case '[': --brackets; break;
					}

					if (brackets == 0) {
						break;
					}
					--current;
				}

				if (brackets > 0) {
					cerr << "program had an unclosed ']'" << endl;
					return 1;
				}
			}
			break;
		}
		++current;
	}
	return 0;
}	
