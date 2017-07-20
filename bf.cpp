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
	static const char allowed[] = {
		'>', '<', '+', '-', '.', ',', '[', ']'
	};
	static const size_t n = sizeof(allowed) / sizeof(*allowed);

	for (size_t i = 0; i < n; ++i) {
		if (c == allowed[i]) {
			return true;
		}
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

// Initialize with 25 cells by default, but allow the caller to change that.
Pointer::Pointer(vector<unsigned char>::size_type n = 25): cells(n, 0), index(0), size(n) {} 

void Pointer::right()
{
	if (index == cells.max_size()-1) {
		throw out_of_range("program tried to access too large of a cell");
	}
	++index;
	// add more cells if necessary
	if (index >= size) {
		cells.push_back(0);
	}
}

void Pointer::left()
{
	if (index == 0) {
		throw out_of_range("program tried to access cell -1");
	}
	--index;
}

void Pointer::increment()
{
	++cells[index];
}

void Pointer::decrement()
{
	--cells[index];
}

void Pointer::write()
{
	cout << cells[index];
}

void Pointer::read()
{
	char c;
	if (!cin.get(c)) {
		c = 0;
	}
	cells[index] = c;
}

int main(int argc, char** argv)
{
	if (argc == 1) {
		cerr << "no file given" << endl;
		return 1;
	}

	Interpreter in(argv[1]);
	Pointer p(10);
	
	while (in.current() != '\0') {
		char command = in.current();

		switch (command) {
		case '>': p.right(); break;
		case '<': p.left(); break;
		case '+': p.increment(); break;
		case '-': p.decrement(); break;
		case '.': p.write(); break;
		case ',': p.read(); break;
		case '[': 
			if (p.current() == 0) {
				in.jumpf();
			}
			break;
		case ']':
			if (p.current() != 0) {
				in.jumpb();
			}
			break;
		}

		in.next();
	}

	return 0;
}	
