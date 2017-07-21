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

	// represents the "address field" - a vector of cells from 0 to
	// infinity, where each cell stores a number up to 255
	vector<unsigned char> cells(3000, 0);
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
					// initially set to 1 to include the starting bracket
					vector<unsigned char>::size_type brackets = 1;

					// stops going forward at eof or when brackets is 0
					while (file.get(command)) {
						switch (command) {
							case '[': ++brackets; break;
							case ']': --brackets; break;
						}

						if (brackets == 0) {
							break;
						}
					}

					if (brackets > 0) {
						cerr << ("program had an unclosed '['") << endl;
						return 1;
					}
				}
				break;
			// loop end
			case ']':
				if (cells[index] != 0) {
					// initially set to 0 because unget() goes over the
					// starting bracket again
					vector<unsigned char>::size_type brackets = 0;

					// stops going backward at beginning of file 
					// (error returns false) or when brackets is 0
					while (file.unget()) {
						command = file.peek();
						switch (command) {
							case ']': ++brackets; break;
							case '[': --brackets; break;
						}

						if (brackets == 0) {
							break;
						}
					}

					if (brackets > 0) {
						cerr << "program had an unclosed ']'" << endl;
						return 1;
					}
				}
				break;
		}
	}
	return 0;
}	
