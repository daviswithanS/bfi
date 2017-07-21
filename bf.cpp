#include <cstddef>
#include <fstream>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
#include "bf.h"

using std::size_t;
using std::ifstream;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::stack;
using std::invalid_argument;
using std::out_of_range;
using std::string;
using std::vector;

Interpreter::Interpreter(const string& filename)
{
	load_instructions(filename); 
	iter = commands.begin();
}

// The load_instructions function parses an input file and stores all of the
// valid brainfuck commands.
void Interpreter::load_instructions(const string& filename)
{
	ifstream file(filename.c_str());
	if (!file) {
		throw invalid_argument("invalid or missing filename");
	}

	char c;
	while (file.get(c)) {		// loop ends if eof or error
		if (is_valid(c)) {
			commands.push_back(c);
		}
	}
	commands.push_back('\0');
}

bool Interpreter::is_valid(const char c)
{
	if (c == '>' || c == '<' || c == '+' || c == '-' ||
			c == '.' || c == ',' || c == '[' || c == ']') {
		return true;
	}
	return false;
}

void Interpreter::jumpf()
{
	stack<char> brackets;
	brackets.push('[');
	// go until end is reached or all brackets are resolved (a pair is found)
	while (iter != commands.end()) {
		++iter;
		switch (*iter) {
			case '[': brackets.push('['); break;
			case ']': brackets.pop(); break;
		}

		if (brackets.size() == 0) {
			break;
		}
	}

	// throw error if any brackets are unresolved
	if (brackets.size() > 0) {
		throw out_of_range("program had an unclosed '['");
	}
}

void Interpreter::jumpb()
{
	stack<char> brackets;
	brackets.push(']');
	// see jumpf()
	while (iter != commands.begin()) {
		--iter;
		switch (*iter) {
			case ']': brackets.push(']'); break;
			case '[': brackets.pop(); break;
		}

		if (brackets.size() == 0) {
			break;
		}
	}

	// throw error if any brackets are unresolved
	if (brackets.size() > 0) {
		throw out_of_range("program had an unclosed ']'");
	}
}

int main(int argc, char** argv)
{
	if (argc == 1) {
		cerr << "no file given" << endl;
		return 1;
	}

	Interpreter in(argv[1]);

	vector<unsigned char>::size_type size = 10;

	// represents the "address field" - a string of cells from 0 to
	// infinity, where each cell stores a number up to 255
	vector<unsigned char> cells(size, 0);
	vector<unsigned char>::size_type index = 0;

	while (in.current() != '\0') {
		char command = in.current();

		switch (command) {
			// move right
			case '>': 
				if (index == cells.max_size()-1) {
					throw out_of_range("program tried to access too large of a cell");
				}
				++index;
				// add more cells if necessary
				if (index >= size) {
					cells.push_back(0);
				}
				break;
			// move left
			case '<': 
				if (index == 0) {
					throw out_of_range("program tried to access cell -1");
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
					in.jumpf();
				}
				break;
			// loop end
			case ']':
				if (cells[index] != 0) {
					in.jumpb();
				}
				break;
		}
		in.next();
	}

	return 0;
}	
